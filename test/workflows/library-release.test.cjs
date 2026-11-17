const { test } = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { resolveRequest, changelogNotes, requestRelease } = require("../../.github/scripts/library-release.cjs");
const config = require("../../.github/library-sync-targets.json");
const root = path.resolve(__dirname, "../..");

function receiverScript(step, file = "build-library-release.yml") {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows", file), "utf8").replace(/\r/g, "");
  const section = workflow.split(`      - name: ${step}\n`)[1].split("\n      - name: ")[0];
  const script = section.split("          script: |\n")[1].split("\n").map(line => line.slice(12)).join("\n");
  const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;
  return new AsyncFunction("github", "context", "core", "require", "console", script);
}

test("release requests resolve enabled remote mappings and validate versions", () => {
  const release = resolveRequest(config, "### Library\n\nArrayList\n\n### Version\n\n1.2.3-beta.1\n\n### Release notes\n\nFixed resizing.\n");
  assert.equal(release.repo, "ArduinoArrayList");
  assert.equal(release.prerelease, true);
  assert.equal(release.notes, "Fixed resizing.");
  assert.throws(() => resolveRequest(config, "### Library\nQueue\n### Version\n1.2.3"), /enabled library/);
  assert.throws(() => resolveRequest(config, "### Library\nArrayList\n### Version\nv1.2.3; echo unsafe"), /semantic version/);
});

test("changelog extraction excludes historical commits and preserves subsections", () => {
  assert.equal(changelogNotes("# Changelog\n\n## Next Release (Unreleased)\n\n### Fixes\n\n- Fixed resize.\n\n| Date | Commit |\n| old | old |"), "### Fixes\n\n- Fixed resize.");
  assert.equal(changelogNotes("No changelog"), "");
});

test("release requests reject unauthorized authors and triggering actors independently", async () => {
  for (const permissions of [
    { author: "read", actor: "admin" },
    { author: "write", actor: "admin" },
    { author: "admin", actor: "write" }
  ]) {
    let dispatched = false;
    const github = { rest: {
      issues: {
        get: async () => ({ data: { labels: [{ name: "library-release-request" }], user: { login: "author" } } }),
        createComment: async () => {}
      },
      repos: { getCollaboratorPermissionLevel: async ({ username }) => ({ data: { permission: permissions[username] } }) }
    } };
    await assert.rejects(requestRelease({
      github, context: { repo: { owner: "owner", repo: "source" }, actor: "actor", eventName: "issues", payload: { issue: { number: 1 } } },
      request: async () => { dispatched = true; }
    }), /permission denied/);
    assert.equal(dispatched, false);
  }
});

test("authorized requests dispatch the mapped repository with changelog notes and report the run link", async () => {
  const comments = [];
  const permissions = [];
  const dispatches = [];
  const github = { rest: {
    issues: {
      get: async () => ({ data: {
        labels: [{ name: "library-release-request" }], user: { login: "author" },
        body: "### Library\nArrayList\n### Version\n1.2.3\n### Release notes\n_No response_",
        html_url: "https://github.com/owner/source/issues/1"
      } }),
      createComment: async options => comments.push(options)
    },
    repos: { getCollaboratorPermissionLevel: async ({ username }) => {
      permissions.push(username);
      return { data: { permission: "maintain" } };
    } }
  } };
  await requestRelease({
    github, root, core: { info() {} },
    context: { repo: { owner: "owner", repo: "source" }, actor: "maintainer", eventName: "workflow_dispatch", payload: { inputs: { issue_number: "#1" } } },
    request: operation => operation({ rest: { repos: { createDispatchEvent: async options => dispatches.push(options) } } })
  });
  assert.deepEqual(permissions, ["author", "maintainer"]);
  assert.equal(dispatches.length, 1);
  assert.equal(dispatches[0].repo, "ArduinoArrayList");
  assert.equal(dispatches[0].client_payload.version, "1.2.3");
  assert.ok(dispatches[0].client_payload.notes.includes("###"));
  assert.match(comments[0].body, /actions\/workflows\/build-library-release.yml/);
});

test("receiver refuses invalid versions and existing tags before checkout", async () => {
  const run = receiverScript("Validate release request");
  const context = {
    repo: { owner: "owner", repo: "remote" }, actor: "maintainer",
    payload: { client_payload: { version: "1.2.3", branch: "main" } }
  };
  const github = { rest: {
    repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission: "maintain" } }) },
    git: { getRef: async () => ({ data: {} }) }
  } };
  await assert.rejects(run(github, context, {}, require), /already exists/);
  context.payload.client_payload.version = "$(echo unsafe)";
  await assert.rejects(run(github, context, {}, require), /Invalid release version/);
});

test("receiver resolves new tags to an immutable commit", async () => {
  const outputs = {};
  await receiverScript("Validate release request")({ rest: {
    repos: {
      getCollaboratorPermissionLevel: async () => ({ data: { permission: "admin" } }),
      getBranch: async () => ({ data: { commit: { sha: "exact-sha" } } })
    },
    git: { getRef: async () => { throw Object.assign(new Error("not found"), { status: 404 }); } }
  } }, {
    repo: { owner: "owner", repo: "remote" }, actor: "maintainer",
    payload: { client_payload: { version: "1.2.3", branch: "main" } }
  }, { setOutput: (name, value) => { outputs[name] = value; } }, require);
  assert.deepEqual(outputs, { sha: "exact-sha", tag: "v1.2.3" });
});

test("every issue form includes all enabled remote libraries", () => {
  const enabled = Object.entries(config.libraries)
    .filter(([, target]) => target.enabled !== false && target.targetRepo).map(([name]) => name).sort();
  for (const file of [
    "library_release_request.yml", "library_sync_request.yml", "bug_report.yml",
    "feature_request.yml", "documentation-request.yml", "general-question.yml"
  ]) {
    const template = fs.readFileSync(path.join(root, ".github/ISSUE_TEMPLATE", file), "utf8").replace(/\r/g, "");
    const choices = [...template.matchAll(/^        - ([A-Za-z0-9_]+)$/gm)].map(match => match[1]);
    for (const library of enabled) assert.ok(choices.includes(library), `${file} is missing ${library}`);
    if (file.startsWith("library_")) assert.deepEqual(choices.sort(), enabled);
  }
});

test("sync request resolution supports every enabled remote repository", async context => {
  const previous = { CONFIG_PATH: process.env.CONFIG_PATH, GITHUB_WORKSPACE: process.env.GITHUB_WORKSPACE };
  process.env.CONFIG_PATH = path.join(root, ".github/library-sync-targets.json");
  process.env.GITHUB_WORKSPACE = root;
  context.after(() => {
    for (const [name, value] of Object.entries(previous)) {
      if (value === undefined) delete process.env[name];
      else process.env[name] = value;
    }
  });
  const run = receiverScript("Resolve library sync request", "create-library-sync-pr.yml");
  for (const [library, target] of Object.entries(config.libraries)) {
    if (target.enabled === false || !target.targetRepo) continue;
    const outputs = {};
    await run({ rest: {
      repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission: "admin" } }) }
    } }, {
      repo: { owner: "owner", repo: "source" }, eventName: "issues",
      payload: { issue: {
        number: 1, user: { login: "maintainer" }, labels: [{ name: "library-sync-request" }],
        body: `### Library\n${library}\n`
      } }
    }, {
      info() {}, setFailed: message => assert.fail(message),
      setOutput: (name, value) => { outputs[name] = value; }
    }, require);
    assert.equal(outputs.should_run, "true", library);
    assert.equal(outputs.target_repo, target.targetRepo, library);
    assert.equal(outputs.source_path, target.sourcePath, library);
  }
});

test("automatic sync requests include all twelve configured remotes and skip disabled libraries", async context => {
  const previous = process.env.CONFIG_PATH;
  process.env.CONFIG_PATH = path.join(root, ".github/library-sync-targets.json");
  context.after(() => {
    if (previous === undefined) delete process.env.CONFIG_PATH;
    else process.env.CONFIG_PATH = previous;
  });
  const enabled = Object.entries(config.libraries).filter(([, target]) => target.enabled !== false && target.targetRepo);
  assert.equal(enabled.length, 12);
  const created = [];
  const mockedRequire = name => name === "child_process"
    ? { execFileSync: () => "Update library" } : require(name);
  await receiverScript("Create or update sync request issues for changed libraries", "auto-library-sync-issues.yml")({
    rest: {
      repos: { compareCommitsWithBasehead: async () => ({ data: { files: [
        ...enabled.map(([, target]) => ({ filename: `${target.sourcePath}/src/library.h` })),
        { filename: "lib/Queue/src/library.h" }
      ] } }) },
      issues: { listForRepo() {}, create: async options => {
        created.push(options);
        return { data: { number: created.length } };
      } }
    },
    paginate: async () => []
  }, {
    repo: { owner: "owner", repo: "source" }, eventName: "push", ref: "refs/heads/main",
    payload: { before: "base", after: "head" }
  }, { info() {}, warning: message => assert.fail(message) }, mockedRequire);
  assert.equal(created.length, enabled.length);
  for (const [library, target] of enabled) {
    const issue = created.find(item => item.body.includes(`### Library\n${library}\n`));
    assert.ok(issue, library);
    assert.ok(issue.title.endsWith(`/${target.targetRepo}`), library);
  }
});

test("manifest mismatches block publication and missing notes are generated remotely", async () => {
  let version = "0.0.1";
  const writes = [];
  const fakeFs = {
    existsSync: file => file === "library.json",
    readFileSync: () => JSON.stringify({ version }),
    mkdirSync() {},
    writeFileSync: (file, content) => writes.push(content)
  };
  const mockedRequire = name => name === "node:fs" ? fakeFs : { join: (...parts) => parts.join("/") };
  const run = receiverScript("Validate manifests and prepare release notes");
  const context = { repo: { owner: "owner", repo: "remote" }, payload: { client_payload: { version: "1.2.3", notes: "" } } };
  const github = { rest: { repos: { generateReleaseNotes: async () => ({ data: { body: "Generated changes." } }) } } };
  await assert.rejects(run(github, context, {}, mockedRequire), /manifest must match/);
  assert.equal(writes.length, 0);
  version = "1.2.3";
  await run(github, context, {}, mockedRequire);
  assert.deepEqual(writes, ["Generated changes."]);
});

test("remote release stays draft until every asset uploads successfully", async () => {
  const calls = [];
  let failUpload = true;
  const run = receiverScript("Publish release and package");
  const github = { rest: { repos: {
    createRelease: async options => {
      calls.push(["create", options.draft]);
      return { data: { id: 1, html_url: "https://example.test/release" } };
    },
    uploadReleaseAsset: async ({ name }) => {
      calls.push(["upload", name]);
      if (failUpload) throw new Error("Upload failed");
    },
    updateRelease: async options => calls.push(["publish", options.draft])
  } } };
  const mockedRequire = name => name === "node:fs" ? {
    readFileSync: () => Buffer.from("content"),
    readdirSync: () => ["library.zip", "library.zip.sha256", "release-notes.md"]
  } : { join: (...parts) => parts.join("/") };
  const context = { repo: { owner: "owner", repo: "remote" }, payload: { client_payload: { library: "Library", version: "1.2.3" } } };
  const core = { summary: { addLink() { return this; }, async write() {} } };
  await assert.rejects(run(github, context, core, mockedRequire), /Upload failed/);
  assert.equal(calls.some(([name]) => name === "publish"), false);
  calls.length = 0;
  failUpload = false;
  await run(github, context, core, mockedRequire);
  assert.deepEqual(calls, [["create", true], ["upload", "library.zip"], ["upload", "library.zip.sha256"], ["publish", false]]);
});

test("release label watchdog enforces maintainer permission on additions, removals, and issue forms", async context => {
  const workflow = fs.readFileSync(path.join(root, ".github/workflows/restricted-label-watchdog.yml"), "utf8");
  const previous = process.env.RESTRICTED_LABELS;
  process.env.RESTRICTED_LABELS = /RESTRICTED_LABELS: (.+)/.exec(workflow)[1].trim();
  context.after(() => {
    if (previous === undefined) delete process.env.RESTRICTED_LABELS;
    else process.env.RESTRICTED_LABELS = previous;
  });
  const run = receiverScript("Guard Restricted Labels", "restricted-label-watchdog.yml");
  for (const action of ["opened", "reopened", "labeled", "unlabeled"]) {
    for (const permission of ["read", "triage", "write", "maintain", "admin"]) {
      const calls = [];
      const label = { name: "library-release-request" };
      await run({ rest: {
        repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission } }) },
        issues: {
          removeLabel: async () => calls.push("remove"),
          addLabels: async () => calls.push("restore")
        }
      } }, {
        repo: { owner: "owner", repo: "source" },
        payload: {
          action, sender: { login: "actor", type: "User" },
          label: ["labeled", "unlabeled"].includes(action) ? label : undefined,
          issue: { number: 1, labels: action === "unlabeled" ? [] : [label] }
        }
      }, {}, require, { log() {} });
      assert.deepEqual(calls, ["admin", "maintain"].includes(permission)
        ? [] : [action === "unlabeled" ? "restore" : "remove"], `${action}: ${permission}`);
    }
  }
  for (const action of ["opened", "reopened"]) {
    assert.match(workflow, new RegExp(`types: \\[.*\\b${action}\\b`));
  }
  const removed = [];
  await run({ rest: {
    repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission: "write" } }) },
    issues: { removeLabel: async () => removed.push("release") }
  } }, {
    repo: { owner: "owner", repo: "source" },
    payload: {
      action: "labeled", sender: { login: "github-actions[bot]", type: "Bot" },
      label: { name: "library-release-request" }, issue: { number: 1 }
    }
  }, {}, require, { log() {} });
  assert.deepEqual(removed, ["release"]);
});

test("release receiver rejects write-only collaborators", async () => {
  await assert.rejects(receiverScript("Validate release request")({ rest: {
    repos: { getCollaboratorPermissionLevel: async () => ({ data: { permission: "write" } }) }
  } }, {
    repo: { owner: "owner", repo: "remote" }, actor: "contributor",
    payload: { client_payload: { version: "1.2.3", branch: "main" } }
  }, {}, require), /Release permission denied/);
});