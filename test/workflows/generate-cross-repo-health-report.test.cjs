const test = require("node:test");
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");

const root = path.resolve(__dirname, "../..");
const AsyncFunction = Object.getPrototypeOf(async function () {}).constructor;

function libraryResolutionScript() {
  const workflow = fs.readFileSync(
    path.join(root, ".github/workflows/generate-cross-repo-health-report.yml"),
    "utf8"
  );
  const step = workflow.split("      - name: Analyze issues and publish report\n")[1]?.split("\n      - name: ")[0];
  assert.ok(step, "Missing report workflow step");
  const script = step.split("          script: |\n")[1];
  assert.ok(script, "Missing report workflow script");
  const source = script.split("\n").map(line => line.slice(12)).join("\n");
  assert.doesNotThrow(
    () => new AsyncFunction("github", "context", "core", "getOctokit", "require", source),
    "Report workflow script must be valid JavaScript"
  );
  const start = source.indexOf("const configuredLibraryLabels = [];");
  const end = source.indexOf("\n\ncore.info(`Loading overrides", start);
  assert.ok(start >= 0 && end > start, "Missing library label resolution logic");
  return new AsyncFunction(
    "templateLibraryLabels",
    "privateRepoLabels",
    "privateGh",
    "privateOwner",
    "privateRepo",
    "core",
    "normalize",
    `${source.slice(start, end)}\nreturn { configuredLibraryLabels, uniqueConfiguredLibraryLabels };`
  );
}

test("health report reuses matching labels and creates missing template labels", async () => {
  const createLabelCalls = [];
  const script = libraryResolutionScript();
  const result = await script(
    ["AVLTree", "ArrayList"],
    [{ name: "AVL Tree" }],
    { rest: { issues: { createLabel: async options => {
      createLabelCalls.push(options);
      return { data: { name: options.name } };
    } } } },
    "owner",
    "repo",
    { info() {}, warning() {} },
    value => (value || "").toLowerCase().replace(/fs/g, "filesystem").replace(/[^a-z0-9]/g, "")
  );

  assert.deepEqual(result.uniqueConfiguredLibraryLabels, ["AVL Tree", "ArrayList"]);
  assert.deepEqual(createLabelCalls, [{
    owner: "owner",
    repo: "repo",
    name: "ArrayList",
    color: "1D76DB",
    description: "Library: ArrayList"
  }]);
});
