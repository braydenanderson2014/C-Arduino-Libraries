const test = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
const { enableCredentialFallback } = require("../../.github/scripts/library-sync-credentials.cjs");

const root = path.resolve(__dirname, "../..");
const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;

function workflowScript(file, step) {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows", file), "utf8");
  const section = workflow.split(`      - name: ${step}\n`)[1]?.split("\n      - name: ")[0];
  assert.ok(section, `Missing step ${step}`);
  const script = section.split("          script: |\n")[1];
  assert.ok(script, `Missing script in ${step}`);
  return new AsyncFunction("github", "context", "core", "getOctokit", "require",
    script.split("\n").map(line => line.slice(12)).join("\n"));
}

test("PR triage uses repository credentials before the project credential", async () => {
  const script = workflowScript("pull-request-triage.yml", "Process Pull Request");
  const calls = [];
  const github = {
    rest: { issues: {
      getLabel: async () => calls.push("label"),
      addLabels: async () => calls.push("add-label"),
      listComments: () => {},
      createComment: async () => calls.push("comment")
    } },
    paginate: async () => []
  };
  const context = { repo: { owner: "owner", repo: "repo" }, payload: { pull_request: { number: 12, labels: [] } } };
  const oldToken = process.env.PROJECT_TOKEN;
  process.env.PROJECT_TOKEN = "project-token";
  try {
    await script(github, context, {}, token => {
      assert.equal(token, "project-token");
      return { graphql: async query => {
        calls.push("project");
        if (query.includes("fields(first: 50)")) {
          return { user: { projectV2: { id: "p", fields: { nodes: [] } } }, repository: { pullRequest: { id: "pr" } } };
        }
        if (query.includes("items(first: 100")) {
          return { node: { items: { nodes: [{ id: "item", content: { id: "pr" } }], pageInfo: { hasNextPage: false } } } };
        }
        throw new Error("Unexpected project query");
      } };
    }, require);
    assert.deepEqual(calls, ["label", "add-label", "comment", "project", "project"]);
  } finally {
    if (oldToken === undefined) delete process.env.PROJECT_TOKEN;
    else process.env.PROJECT_TOKEN = oldToken;
  }
});

test("library request resolution uses the repository client", async () => {
  const script = workflowScript("create-library-sync-pr.yml", "Resolve library sync request");
  const outputs = {};
  const oldWorkspace = process.env.GITHUB_WORKSPACE;
  const oldConfig = process.env.CONFIG_PATH;
  process.env.GITHUB_WORKSPACE = root;
  process.env.CONFIG_PATH = path.join(root, ".github/library-sync-targets.json");
  try {
    await script({
      rest: { repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission: "admin" } }) } }
    }, {
      eventName: "issues",
      repo: { owner: "owner", repo: "repo" },
      payload: { issue: { number: 17, user: { login: "maintainer" }, labels: [{ name: "library-sync-request" }], body: "### Library\nArrayList\n" } }
    }, { setOutput: (name, value) => { outputs[name] = value; } }, () => {
      throw new Error("A second client should not be needed");
    }, require);
    assert.equal(outputs.should_run, "true");
    assert.equal(outputs.target_repo, "ArduinoArrayList");
  } finally {
    if (oldWorkspace === undefined) delete process.env.GITHUB_WORKSPACE;
    else process.env.GITHUB_WORKSPACE = oldWorkspace;
    if (oldConfig === undefined) delete process.env.CONFIG_PATH;
    else process.env.CONFIG_PATH = oldConfig;
  }
});

test("remote API retries an access rejection with the alternate token", async () => {
  let hook;
  const warnings = [];
  const github = { hook: { wrap: (_, fn) => { hook = fn; } } };
  enableCredentialFallback(github, { setSecret() {}, warning: message => warnings.push(message), error() {} },
    { LIBRARY_SYNC_TOKEN: "expired", GH_PROJECT_TOKEN: "working" });
  const attempted = [];
  const response = await hook(async options => {
    attempted.push(options.headers.authorization);
    if (attempted.length === 1) throw Object.assign(new Error("Bad credentials"), { status: 401 });
    return "success";
  }, { headers: {} });
  assert.equal(response, "success");
  assert.deepEqual(attempted, ["token expired", "token working"]);
  assert.equal(warnings.length, 1);
});

test("remote PR creation reports to the source issue using its repository token", async () => {
  const script = workflowScript("create-library-sync-pr.yml", "Create or update remote pull request");
  const keys = ["GITHUB_WORKSPACE", "LIBRARY_SYNC_TOKEN", "GH_PROJECT_TOKEN", "REPOSITORY_TOKEN", "ISSUE_NUMBER", "LIBRARY_NAME", "TARGET_OWNER", "TARGET_REPO", "BASE_BRANCH", "BRANCH_NAME", "PR_TITLE", "PR_BODY"];
  const previous = Object.fromEntries(keys.map(key => [key, process.env[key]]));
  Object.assign(process.env, {
    GITHUB_WORKSPACE: root, LIBRARY_SYNC_TOKEN: "expired", GH_PROJECT_TOKEN: "working",
    REPOSITORY_TOKEN: "repository", ISSUE_NUMBER: "17", LIBRARY_NAME: "ArrayList",
    TARGET_OWNER: "owner", TARGET_REPO: "ArduinoArrayList", BASE_BRANCH: "main",
    BRANCH_NAME: "sync/17", PR_TITLE: "sync", PR_BODY: "body"
  });
  let hook;
  const attempts = [];
  let comment;
  const remoteRequest = async () => hook(async options => {
    attempts.push(options.headers.authorization);
    if (options.headers.authorization === "token expired") throw Object.assign(new Error("Bad credentials"), { status: 401 });
    return { data: { html_url: "https://github.com/owner/ArduinoArrayList/pull/1" } };
  }, { headers: {} });
  const github = {
    hook: { wrap: (_, fn) => { hook = fn; } },
    paginate: async () => [],
    rest: { pulls: { list() {}, create: remoteRequest } }
  };
  try {
    await script(github, { repo: { owner: "owner", repo: "monorepo" } },
      { setSecret() {}, warning() {}, error() {} },
      token => {
        assert.equal(token, "repository");
        return { rest: { issues: { createComment: async options => { comment = options; } } } };
      }, require);
    assert.deepEqual(attempts, ["token expired", "token working"]);
    assert.equal(comment.repo, "monorepo");
    assert.equal(comment.issue_number, 17);
    assert.match(comment.body, /ArduinoArrayList\/pull\/1/);
  } finally {
    for (const key of keys) {
      if (previous[key] === undefined) delete process.env[key];
      else process.env[key] = previous[key];
    }
  }
});

test("remote Git retries authentication failures but not other failures", () => {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "library-sync-test-"));
  try {
    const fakeGit = path.join(dir, "git");
    const attempts = path.join(dir, "attempts");
    fs.writeFileSync(fakeGit, `#!/usr/bin/env node
const fs = require("node:fs");
const value = Buffer.from(process.env.GIT_CONFIG_VALUE_0.split(" ").pop(), "base64").toString();
fs.appendFileSync(process.env.ATTEMPTS, value + "\\n");
if (process.env.FAILURE === "network") { console.error("Could not resolve host"); process.exit(128); }
if (value.endsWith(":expired")) { console.error("fatal: could not read Username for 'https://github.com': terminal prompts disabled"); process.exit(128); }
`, { mode: 0o755 });
    const env = {
      ...process.env, PATH: `${dir}:${process.env.PATH}`, ATTEMPTS: attempts,
      LIBRARY_SYNC_TOKEN: "expired", GH_PROJECT_TOKEN: "working"
    };
    const command = path.join(root, ".github/scripts/library-sync-git.cjs");
    const result = spawnSync(process.execPath, [command, "clone", "https://github.com/owner/repo.git", "repo"], { env, encoding: "utf8" });
    assert.equal(result.status, 0, result.stderr);
    assert.deepEqual(fs.readFileSync(attempts, "utf8").trim().split("\n"), ["x-access-token:expired", "x-access-token:working"]);
    fs.writeFileSync(attempts, "");
    const failure = spawnSync(process.execPath, [command, "push", "origin", "branch"], { env: { ...env, FAILURE: "network" }, encoding: "utf8" });
    assert.equal(failure.status, 1);
    assert.match(failure.stderr, /Could not resolve host/);
    assert.equal(fs.readFileSync(attempts, "utf8").trim(), "x-access-token:expired");
  } finally {
    fs.rmSync(dir, { recursive: true, force: true });
  }
});
