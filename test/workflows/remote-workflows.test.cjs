const test = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
const { loadPlan, syncTarget, syncWorkflows } = require("../../.github/scripts/sync-remote-workflows.cjs");

const root = path.resolve(__dirname, "../..");
const source = "owner/monorepo";

function fixture(t) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), "workflow-sync-test-"));
  t.after(() => fs.rmSync(dir, { recursive: true, force: true }));
  fs.mkdirSync(path.join(dir, ".github/workflows"), { recursive: true });
  fs.writeFileSync(path.join(dir, ".github/workflows/shared.yml"), "name: Shared\n");
  fs.writeFileSync(path.join(dir, ".github/workflow-sync-config.json"), JSON.stringify({
    targetsConfig: ".github/targets.json", files: [".github/workflows/shared.yml"]
  }));
  const writeTargets = libraries => fs.writeFileSync(path.join(dir, ".github/targets.json"), JSON.stringify({
    defaults: { targetOwner: "owner", baseBranch: "main" }, libraries
  }));
  writeTargets({ Library: { targetRepo: "library" } });
  return { dir, writeTargets };
}

function mockGithub(tree = [], failUpdate = false) {
  const calls = [];
  const method = (name, data) => async options => {
    calls.push({ name, ...options });
    if (name === "updateRef" && failUpdate) throw new Error("Not a fast forward");
    return { data };
  };
  return {
    calls,
    github: { rest: { git: {
      getRef: method("getRef", { object: { sha: "parent" } }),
      getCommit: method("getCommit", { tree: { sha: "base-tree" } }),
      getTree: method("getTree", { tree, truncated: false }),
      createBlob: method("createBlob", { sha: "blob" }),
      createTree: method("createTree", { sha: "next-tree" }),
      createCommit: method("createCommit", { sha: "commit" }),
      updateRef: method("updateRef", {})
    } } }
  };
}

test("sync plan reuses library defaults and overrides, skips disabled targets, and deduplicates aliases", t => {
  const { dir, writeTargets } = fixture(t);
  writeTargets({
    First: { targetRepo: "library" },
    Alias: { targetRepo: "library" },
    Override: { targetOwner: "another", targetRepo: "second", baseBranch: "release/1" },
    Disabled: { enabled: false }
  });
  assert.deepEqual(loadPlan(dir, source).targets, [
    { owner: "owner", repo: "library", branch: "main" },
    { owner: "another", repo: "second", branch: "release/1" }
  ]);
  writeTargets({ Self: { targetRepo: "monorepo" } });
  assert.throws(() => loadPlan(dir, source), /source repository/);
});

test("configured workflow sync targets include the available library repositories", () => {
  const { targets } = loadPlan(root, "braydenanderson2014/C-Arduino-Libraries");
  assert.deepEqual(targets.map(target => target.repo).sort(), [
    "ArduinoArrayList", "ArduinoDoubleLinkedList", "ArduinoHashtable", "ArduinoLinkedList",
    "ArduinoSDList", "ArduinoTimer", "ExtremeVariant", "Optional", "SimpleProperties",
    "SimpleVector", "UnorderedMap", "Variant"
  ].sort());
});

test("missing configured source files fail before contacting any remote", async t => {
  const { dir } = fixture(t);
  fs.unlinkSync(path.join(dir, ".github/workflows/shared.yml"));
  const { github, calls } = mockGithub();
  await assert.rejects(syncWorkflows({
    github, core: {}, context: { repo: { owner: "owner", repo: "monorepo" } }, root: dir
  }), /ENOENT/);
  assert.equal(calls.length, 0);
});

test("sync commits only changed configured files on top of the remote branch", async t => {
  const { dir } = fixture(t);
  const { files, targets } = loadPlan(dir, source);
  const { github, calls } = mockGithub([{ path: "README.md", sha: "unrelated", mode: "100644" }]);
  assert.equal(await syncTarget(github, targets[0], files, source), true);
  const blob = calls.find(call => call.name === "createBlob");
  assert.equal(Buffer.from(blob.content, blob.encoding).toString(), "name: Shared\n");
  const tree = calls.find(call => call.name === "createTree");
  assert.equal(tree.base_tree, "base-tree");
  assert.deepEqual(tree.tree, [{ path: files[0].path, mode: "100644", type: "blob", sha: "blob" }]);
  const commit = calls.find(call => call.name === "createCommit");
  assert.deepEqual(commit.parents, ["parent"]);
  const update = calls.find(call => call.name === "updateRef");
  assert.equal(update.force, false);
  assert.equal(update.ref, "heads/main");
});

test("identical remote files produce no writes", async t => {
  const { dir } = fixture(t);
  const { files, targets } = loadPlan(dir, source);
  const { github, calls } = mockGithub(files.map(file => ({ path: file.path, sha: file.sha, mode: "100644" })));
  assert.equal(await syncTarget(github, targets[0], files, source), false);
  assert.deepEqual(calls.map(call => call.name), ["getRef", "getCommit", "getTree"]);
});

test("a concurrent update fails without retrying a force push", async t => {
  const { dir } = fixture(t);
  const { files, targets } = loadPlan(dir, source);
  const { github, calls } = mockGithub([], true);
  await assert.rejects(syncTarget(github, targets[0], files, source), /Not a fast forward/);
  assert.equal(calls.filter(call => call.name === "updateRef").length, 1);
  assert.equal(calls.at(-1).force, false);
});

test("one inaccessible target does not prevent attempts to sync the remaining targets", async t => {
  const { dir, writeTargets } = fixture(t);
  writeTargets({ First: { targetRepo: "missing" }, Second: { targetRepo: "working" } });
  const { github, calls } = mockGithub();
  const getRef = github.rest.git.getRef;
  github.rest.git.getRef = options => {
    if (options.repo === "missing") throw new Error("Repository not found");
    return getRef(options);
  };
  const errors = [];
  await assert.rejects(syncWorkflows({
    github, core: { error: message => errors.push(message), info() {} },
    context: { repo: { owner: "owner", repo: "monorepo" }, sha: "source-sha" }, root: dir
  }), /failed for 1 repositories: owner\/missing@main/);
  assert.match(errors[0], /Repository not found/);
  assert.equal(calls.at(-1).repo, "working");
  assert.equal(calls.at(-1).name, "updateRef");
});

test("workflow uses credential fallback for individual remote API calls", async t => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/sync-remote-workflows.yml"), "utf8");
  const section = workflow.split("      - name: Sync configured files to library repositories\n")[1];
  const script = section.split("          script: |\n")[1].split("\n").map(line => line.slice(12)).join("\n");
  const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;
  const run = new AsyncFunction("github", "context", "core", "getOctokit", "require", script);

  for (const scenario of ["authorization", "network"]) {
    await t.test(scenario, async t => {
      const { dir } = fixture(t);
      fs.symlinkSync(path.join(root, ".github/scripts"), path.join(dir, ".github/scripts"), "dir");
      const keys = ["GITHUB_WORKSPACE", "LIBRARY_SYNC_TOKEN", "GH_PROJECT_TOKEN", "SYNC_TOKEN", "SOURCE_SHA"];
      const previous = Object.fromEntries(keys.map(key => [key, process.env[key]]));
      Object.assign(process.env, {
        GITHUB_WORKSPACE: dir, LIBRARY_SYNC_TOKEN: "expired", GH_PROJECT_TOKEN: "working",
        SYNC_TOKEN: "", SOURCE_SHA: "source-sha"
      });
      t.after(() => {
        for (const key of keys) {
          if (previous[key] === undefined) delete process.env[key];
          else process.env[key] = previous[key];
        }
      });

      const { github, calls } = mockGithub();
      const attempts = [];
      const secrets = [];
      const core = { setSecret: token => secrets.push(token), warning() {}, error() {}, info() {} };
      const getOctokit = token => ({ rest: { git: Object.fromEntries(
        Object.entries(github.rest.git).map(([name, method]) => [name, async options => {
          attempts.push(`${token}:${name}`);
          if (token === "expired" && (name === "getRef" || name === "updateRef")) {
            const error = new Error(scenario === "network" && name === "updateRef" ? "Network error" : "Forbidden");
            if (error.message === "Forbidden") error.status = 403;
            throw error;
          }
          return method(options);
        }])
      ) } });
      class Context {
        get repo() {
          return { owner: "owner", repo: "monorepo" };
        }
      }
      const context = new Context();
      context.sha = "event-sha";
      const execute = () => run({}, context, core, getOctokit, require);
      if (scenario === "authorization") {
        await execute();
        assert.deepEqual(attempts.slice(0, 3), ["expired:getRef", "working:getRef", "expired:getCommit"]);
        assert.deepEqual(attempts.slice(-2), ["expired:updateRef", "working:updateRef"]);
        assert.equal(calls.filter(call => call.name === "createCommit").length, 1);
        assert.match(calls.find(call => call.name === "createCommit").message, /owner\/monorepo@source-sha/);
      } else {
        await assert.rejects(execute(), /Workflow sync failed/);
        assert.equal(attempts.at(-1), "expired:updateRef");
        assert.ok(!attempts.includes("working:updateRef"));
      }
      assert.ok(secrets.includes("expired") && secrets.includes("working"));
    });
  }
});

test("Arduino lint supports standalone roots, monorepo libraries, empty repositories, and nonzero exits", async t => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/check-arduino.yml"), "utf8");
  const section = workflow.split("      - name: Run Arduino Lint and capture logs\n")[1].split("\n      - name: ")[0];
  const script = section.split("        run: |\n")[1].split("\n").map(line => line.slice(10)).join("\n");
  for (const scenario of ["root", "monorepo", "empty", "failure"]) {
    await t.test(scenario, t => {
      const { dir } = fixture(t);
      if (scenario !== "empty") {
        const library = scenario === "monorepo" ? path.join(dir, "lib/Example") : dir;
        fs.mkdirSync(library, { recursive: true });
        fs.writeFileSync(path.join(library, "library.properties"), "name=Example\n");
      }
      const bin = path.join(dir, "bin");
      fs.mkdirSync(bin);
      fs.writeFileSync(path.join(bin, "arduino-lint"), '#!/bin/sh\nprintf "%s\\n" "$@" >> "$LINT_ARGS"\nexit "$LINT_EXIT"\n', { mode: 0o755 });
      const output = path.join(dir, "output");
      const args = path.join(dir, "args");
      const result = spawnSync("bash", ["-e", "-o", "pipefail", "-c", script], {
        cwd: dir, encoding: "utf8", env: {
          ...process.env, PATH: `${bin}:${process.env.PATH}`, GITHUB_OUTPUT: output,
          LINT_ARGS: args, LINT_EXIT: scenario === "failure" ? "2" : "0"
        }
      });
      assert.equal(result.status, 0, result.stderr);
      const failed = scenario === "failure" || scenario === "empty";
      assert.match(fs.readFileSync(output, "utf8"), new RegExp(`has_errors=${failed}`));
      if (scenario !== "empty") {
        assert.deepEqual(fs.readFileSync(args, "utf8").trim().split("\n"), [
          "--compliance", "specification",
          "--library-manager", "update",
          "--project-type", "library",
          scenario === "monorepo" ? "lib/Example" : "."
        ]);
      }
    });
  }
});

test("Arduino lint installation uses the upstream script and fails on download or missing binary", t => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/check-arduino.yml"), "utf8");
  const section = workflow.split("      - name: Install Arduino Lint\n")[1].split("\n      - name: ")[0];
  assert.match(section, /shell: bash/);
  const script = section.split("        run: |\n")[1].split("\n").map(line => line.slice(10)).join("\n");
  assert.match(script, /arduino-lint\/main\/etc\/install\.sh/);
  for (const scenario of ["success", "download-failure", "missing-binary"]) {
    const { dir } = fixture(t);
    const args = path.join(dir, "args");
    fs.writeFileSync(path.join(dir, "curl"), `#!/bin/sh\nexit ${scenario === "download-failure" ? 22 : 0}\n`, { mode: 0o755 });
    fs.writeFileSync(path.join(dir, "arduino-lint"),
      `#!/bin/sh\nprintf '%s' "$*" > "$LINT_ARGS"\nexit ${scenario === "missing-binary" ? 127 : 0}\n`, { mode: 0o755 });
    const result = spawnSync("bash", ["-e", "-o", "pipefail", "-c", script], {
      encoding: "utf8", env: { ...process.env, PATH: `${dir}:${process.env.PATH}`, LINT_ARGS: args }
    });
    assert.equal(result.status, scenario === "success" ? 0 : scenario === "download-failure" ? 22 : 127, result.stderr);
    if (scenario === "download-failure") assert.equal(fs.existsSync(args), false);
    else assert.equal(fs.readFileSync(args, "utf8"), "--version");
  }
});

test("lint issue creation and updates include source library, project, run, and logs", async () => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/check-arduino.yml"), "utf8");
  const section = workflow.split("      - name: Create or update lint issue on errors\n")[1].split("\n      - name: ")[0];
  const script = section.split("          script: |\n")[1].split("\n").map(line => line.slice(12)).join("\n");
  const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;
  const run = new AsyncFunction("github", "context", "require", "process", script);
  for (const existing of [false, true]) {
    const reports = [];
    await run({ rest: { issues: {
      listForRepo: async () => ({ data: existing ? [{ title: "Arduino Lint errors detected", number: 12 }] : [] }),
      create: async options => reports.push(options),
      createComment: async options => reports.push(options)
    } } }, {
      repo: { owner: "braydenanderson2014", repo: "ArduinoLinkedList" },
      serverUrl: "https://github.com", runId: 123, sha: "abc123", eventName: "push"
    }, () => ({ readFileSync: () => "Linting .\nERROR: example finding" }),
    { env: { ISSUE_TITLE: "Arduino Lint errors detected" } });
    assert.equal(reports.length, 1);
    assert.match(reports[0].body, /### Repository \/ Library\nLinkedList/);
    assert.match(reports[0].body, /Source repository: braydenanderson2014\/ArduinoLinkedList/);
    assert.match(reports[0].body, /Project: https:\/\/github.com\/users\/braydenanderson2014\/projects\/4/);
    assert.match(reports[0].body, /ArduinoLinkedList\/actions\/runs\/123/);
    assert.match(reports[0].body, /Commit: abc123/);
    assert.match(reports[0].body, /ERROR: example finding/);
    if (existing) assert.equal(reports[0].issue_number, 12);
    else assert.deepEqual(reports[0].labels, ["bug", "ci"]);
  }
});

test("scheduled and manual forwarding recover missed bot issues without duplicating mirrors", async () => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/forward-issues-to-main.yml"), "utf8");
  const job = workflow.split("  forward-issue:\n")[1].split("\n  sync-mirrors:")[0];
  assert.match(job.split("    runs-on:")[0], /github\.event_name == 'schedule'/);
  const script = job.split("          script: |\n")[1].split("\n").map(line => line.slice(12)).join("\n");
  const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;
  const run = new AsyncFunction("github", "context", "core", "process", script);
  for (const eventName of ["schedule", "workflow_dispatch", "issues"]) {
    const source = {
      number: 12, node_id: "source", title: "Arduino Lint errors detected",
      body: "### Repository / Library\nLinkedList", html_url: "https://github.com/owner/remote/issues/12",
      user: { login: "github-actions[bot]" }, labels: [{ name: "ci" }]
    };
    const created = [];
    const comments = [];
    const updates = [];
    let mirrored = false;
    const listIssues = () => {};
    const listPulls = () => {};
    const github = {
      paginate: async (method, options) => {
        assert.equal(options.state, "open");
        if (method === listIssues) return [source, { ...source, number: 13, pull_request: {} }];
        assert.equal(method, listPulls);
        return [];
      },
      graphql: async () => ({}),
      rest: {
        pulls: { list: listPulls },
        search: { issuesAndPullRequests: async () => ({ data: { items: mirrored ? [{ number: 99 }] : [] } }) },
        issues: {
          listForRepo: listIssues,
          create: async options => {
            created.push(options);
            return { data: { number: 99, node_id: "mirror", html_url: "https://github.com/target/issues/99" } };
          },
          addLabels: async () => {},
          createComment: async options => comments.push(options),
          update: async options => updates.push(options)
        }
      }
    };
    const core = { getInput: () => "", info() {}, warning() {} };
    const context = { repo: { owner: "owner", repo: "remote" }, eventName, payload: { issue: source } };
    const process = { env: { GH_PROJECT_TOKEN_VALUE: "configured" } };
    await run(github, context, core, process);
    assert.equal(created.length, 1);
    assert.equal(created[0].repo, "C-Arduino-Libraries");
    assert.match(created[0].body, /mirror-source: owner\/remote#12;type=issue/);
    assert.match(created[0].body, /### Repository \/ Library\nLinkedList/);
    assert.equal(updates[0].issue_number, 12);
    assert.equal(updates[0].state, "closed");
    mirrored = true;
    await run(github, context, core, process);
    assert.equal(created.length, 1);
    assert.equal(comments.length, eventName === "issues" ? 3 : 2);
    await run(github, { ...context, repo: { owner: "braydenanderson2014", repo: "C-Arduino-Libraries" } }, core, process);
    await run(github, context, core, { env: {} });
    assert.equal(created.length, 1);
  }
});

test("distributed workflows contain syntactically valid github-script steps", () => {
  const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;
  const { files } = loadPlan(root, "braydenanderson2014/C-Arduino-Libraries");
  for (const file of files.filter(file => /\.ya?ml$/.test(file.path))) {
    const text = Buffer.from(file.content, "base64").toString();
    for (const match of text.matchAll(/^          script: \|\n((?: {12}[^\n]*\n|\n)*)/gm)) {
      const script = match[1].split("\n").map(line => line.slice(12)).join("\n");
      assert.doesNotThrow(() => new AsyncFunction("github", "context", "core", "require", script), file.path);
    }
  }
});
