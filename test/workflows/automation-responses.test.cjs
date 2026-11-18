const test = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { createRequire } = require("node:module");
const { isAutomationIssue } = require("../../.github/scripts/automation-issue.cjs");

const root = path.resolve(__dirname, "../..");
const workflowRequire = createRequire(path.join(root, "workflow.cjs"));
const quiet = { log() {}, error() {}, warn() {} };
const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;

function script(file) {
  const text = fs.readFileSync(path.join(root, ".github/workflows", file), "utf8");
  const body = text.split("script: |\n")[1];
  assert.ok(body, file);
  const indent = body.match(/^ */)[0].length;
  return body.split("\n").map(line => line.slice(indent)).join("\n");
}

const human = { login: "reporter", type: "User" };
const generated = [
  { user: { login: "github-actions[bot]", type: "Bot" }, body: "Lint error" },
  { user: { login: "tool[bot]", type: "User" }, body: "" },
  { user: human, body: "This issue was forwarded from a issue: o/r#7.\n<!-- mirror-source: o/r#7;type=issue -->" },
  { user: human, body: "<!-- library-sync-request -->\n<!-- auto-library-sync-request -->" }
];

test("automation classification recognizes bots and generated issue markers, not manual sync requests", () => {
  for (const issue of generated) assert.equal(isAutomationIssue(issue), true);
  assert.equal(isAutomationIssue({ user: human, body: "<!-- library-sync-request -->" }), false);
  assert.equal(isAutomationIssue({ user: human, body: "A human bug report" }), false);
  assert.equal(isAutomationIssue({}), false);
});

test("welcome responses skip generated issues for opened, label and command events", async () => {
  const run = new AsyncFunction("github", "context", "require", "console", script("welcome-messages.yml"));
  for (const event of ["opened", "labeled", "comment"]) {
    for (const input of [...generated, { user: human, body: "A human bug report" }]) {
      const issue = { ...input, number: 42, title: "Bug", labels: [{ name: "bug" }] };
      const context = {
        repo: { owner: "owner", repo: "repo" },
        eventName: event === "comment" ? "issue_comment" : "issues",
        payload: { issue, action: event, label: { name: "send-welcome" }, comment: { body: "/welcome" } }
      };
      const comments = [];
      const github = { rest: { issues: {
        get: async () => ({ data: issue }),
        createComment: async options => comments.push(options),
        addLabels: async () => {},
        removeLabel: async () => {}
      } } };
      await run(github, context, workflowRequire, quiet);
      assert.equal(comments.length, isAutomationIssue(issue) ? 0 : 1, `${event}: ${input.body}`);
    }
  }
});

test("first-reporter, information and sponsoring responses skip generated issues", async () => {
  for (const [file, label] of [
    ["good-first-issue-triage.yml", null],
    ["needs-more-info.yml", "needs-more-info"],
    ["sponsoring-handler.yml", "sponsor-request"]
  ]) {
    const run = new AsyncFunction("github", "context", "require", "console", script(file));
    for (const input of [...generated, { user: human, body: "Human report" }]) {
      const issue = { ...input, number: 42, title: "Bug", labels: [] };
      const comments = [];
      const github = {
        paginate: async () => [],
        rest: {
          issues: {
            getLabel: async () => {},
            addLabels: async () => {},
            listComments: async () => ({ data: [] }),
            createComment: async options => comments.push(options)
          },
          search: { issuesAndPullRequests: async () => ({ data: { total_count: 1 } }) }
        }
      };
      await run(github, {
        repo: { owner: "owner", repo: "repo" },
        payload: { issue, action: label ? "labeled" : "opened", label: { name: label } }
      }, workflowRequire, quiet);
      assert.equal(comments.length, isAutomationIssue(issue) ? 0 : 1, `${file}: ${input.body}`);
    }
  }
});

test("status updates still run but do not comment on generated issues", async () => {
  for (const [file, label, status] of [
    ["question-status.yml", "question", "General Question"],
    ["documentation-status.yml", "documentation", "Documentation Request"],
    ["working-status.yml", "working", "(Issue) In Progress"]
  ]) {
    const run = new AsyncFunction("github", "context", "require", "console", script(file));
    for (const input of [generated[2], { user: human, body: "Human report" }]) {
      const issue = { ...input, number: 42, node_id: "issue", labels: [{ name: label }] };
      let updates = 0;
      const comments = [];
      const github = {
        graphql: async query => {
          if (query.includes("updateProjectV2ItemFieldValue")) { updates++; return {}; }
          return { user: { projectV2: {
            id: "project",
            items: { nodes: [{ id: "item", content: { number: 42 } }] },
            fields: { nodes: [{ id: "field", name: "Status", options: [{ id: "option", name: status }] }] }
          } } };
        },
        rest: { issues: { createComment: async value => comments.push(value) } }
      };
      await run(github, { repo: { owner: "owner", repo: "repo" }, payload: {
        issue, action: "labeled", label: { name: label }
      } }, workflowRequire, quiet);
      assert.equal(updates, 1, file);
      assert.equal(comments.length, isAutomationIssue(issue) ? 0 : 1, file);
    }
  }
});

test("response workflows contain valid github-script code and load the shared guard", () => {
  for (const file of [
    "welcome-messages.yml", "good-first-issue-triage.yml", "duplicate-detection.yml",
    "needs-more-info.yml", "sponsoring-handler.yml", "question-status.yml",
    "documentation-status.yml", "working-status.yml", "issue-reopen-handler.yml",
    "challenge-handler.yml"
  ]) {
    const body = script(file);
    assert.match(body, /isAutomationIssue\(issue\)/, file);
    assert.doesNotThrow(() => new AsyncFunction("github", "context", "require", "console", body), file);
  }
});

test("UnorderedMap sync replaces examples and removes the legacy remote folder", t => {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "unordered-map-sync-"));
  t.after(() => fs.rmSync(dir, { recursive: true, force: true }));
  const remote = path.join(dir, "remote-repo");
  fs.mkdirSync(path.join(remote, "Example"), { recursive: true });
  fs.writeFileSync(path.join(remote, "Example", "Example.cpp"), "");
  const config = require("../../.github/library-sync-targets.json");
  const paths = config.libraries.UnorderedMap.managedPaths;
  assert.ok(config.defaults.managedPaths.every(entry => paths.includes(entry)));
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/create-library-sync-pr.yml"), "utf8");
  const body = workflow.split("          node <<'NODE'\n")[1].split("\n          NODE")[0]
    .split("\n").map(line => line.slice(10)).join("\n");
  new Function("require", "process", body)(require, { env: {
    GITHUB_WORKSPACE: root, SOURCE_PATH: "lib/UnorderedMap",
    RUNNER_TEMP: dir, TARGET_PATH: ".", MANAGED_PATHS_JSON: JSON.stringify(paths)
  } });
  assert.equal(fs.existsSync(path.join(remote, "Example")), false);
  const example = path.join(remote, "examples/Example/Example.ino");
  assert.ok(fs.existsSync(example));
  assert.match(fs.readFileSync(example, "utf8"), /void setup\(\)[\s\S]*void loop\(\)/);
  assert.deepEqual(JSON.parse(fs.readFileSync(path.join(remote, "library.json"), "utf8")).examples[0].files, ["Example.ino"]);
});
