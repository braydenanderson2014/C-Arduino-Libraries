const fs = require("node:fs");
const path = require("node:path");
const { createHash } = require("node:crypto");

function loadPlan(root, sourceRepository) {
  const readJson = file => JSON.parse(fs.readFileSync(path.join(root, file), "utf8"));
  const localPath = file => {
    if (typeof file !== "string" || !file.startsWith(".github/") ||
        file.split("/").some(part => !part || part === "." || part === "..") || file.includes("\\")) {
      throw new Error(`Expected a repository-relative .github/ path, got ${JSON.stringify(file)}`);
    }
    return file;
  };
  const config = readJson(".github/workflow-sync-config.json");
  if (!Array.isArray(config.files) || !config.files.length) {
    throw new Error("workflow-sync-config.json must contain a nonempty files array.");
  }
  const files = [...new Set(config.files.map(localPath))].map(file => {
    const content = fs.readFileSync(path.join(root, file));
    const sha = createHash("sha1").update(`blob ${content.length}\0`).update(content).digest("hex");
    return { path: file, content: content.toString("base64"), sha };
  });
  const targetsConfig = readJson(localPath(config.targetsConfig));
  if (!targetsConfig.libraries || typeof targetsConfig.libraries !== "object" || Array.isArray(targetsConfig.libraries)) {
    throw new Error("The targets configuration must contain a libraries object.");
  }
  const defaults = targetsConfig.defaults || {};
  const targets = new Map();
  for (const [library, target] of Object.entries(targetsConfig.libraries)) {
    if (target.enabled === false) continue;
    const owner = target.targetOwner || defaults.targetOwner;
    const repo = target.targetRepo;
    const branch = target.baseBranch || defaults.baseBranch || "main";
    if (typeof owner !== "string" || !/^[A-Za-z0-9-]+$/.test(owner) ||
        typeof repo !== "string" || !/^[A-Za-z0-9_.-]+$/.test(repo) ||
        typeof branch !== "string" || !branch.trim()) {
      throw new Error(`Invalid workflow sync target for ${library}.`);
    }
    const repository = `${owner}/${repo}`.toLowerCase();
    if (repository === sourceRepository.toLowerCase()) {
      throw new Error(`Workflow sync target ${library} points at the source repository.`);
    }
    targets.set(`${repository}:${branch}`, { owner, repo, branch });
  }
  if (!targets.size) throw new Error("No enabled workflow sync targets configured.");
  return { files, targets: [...targets.values()] };
}

async function syncTarget(github, target, files, source) {
  const { owner, repo, branch } = target;
  const repository = { owner, repo };
  const { data: ref } = await github.rest.git.getRef({ ...repository, ref: `heads/${branch}` });
  const { data: parent } = await github.rest.git.getCommit({ ...repository, commit_sha: ref.object.sha });
  const { data: tree } = await github.rest.git.getTree({
    ...repository, tree_sha: parent.tree.sha, recursive: "1"
  });
  if (tree.truncated) throw new Error("Remote tree is truncated; refusing an incomplete comparison.");
  const existing = new Map(tree.tree.map(entry => [entry.path, entry]));
  const changed = files.filter(file => {
    const entry = existing.get(file.path);
    return entry?.sha !== file.sha || entry?.mode !== "100644";
  });
  if (!changed.length) return false;

  const entries = [];
  for (const file of changed) {
    const { data: blob } = await github.rest.git.createBlob({
      ...repository, content: file.content, encoding: "base64"
    });
    entries.push({ path: file.path, mode: "100644", type: "blob", sha: blob.sha });
  }
  const { data: nextTree } = await github.rest.git.createTree({
    ...repository, base_tree: parent.tree.sha, tree: entries
  });
  const { data: commit } = await github.rest.git.createCommit({
    ...repository,
    message: `chore(ci): sync shared workflows from ${source}\n\nCo-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>`,
    tree: nextTree.sha,
    parents: [ref.object.sha]
  });
  // A concurrent remote update must fail rather than overwrite unrelated work.
  await github.rest.git.updateRef({
    ...repository, ref: `heads/${branch}`, sha: commit.sha, force: false
  });
  return true;
}

async function syncWorkflows({ github, core, context, root = process.env.GITHUB_WORKSPACE || process.cwd() }) {
  const sourceRepository = `${context.repo.owner}/${context.repo.repo}`;
  const { targets, files } = loadPlan(root, sourceRepository);
  const failures = [];
  for (const target of targets) {
    const name = `${target.owner}/${target.repo}@${target.branch}`;
    try {
      const changed = await syncTarget(github, target, files, `${sourceRepository}@${context.sha}`);
      core.info(`${name}: ${changed ? "synced" : "already up to date"}`);
    } catch (error) {
      core.error(`${name}: ${error.message}`);
      failures.push(name);
    }
  }
  if (failures.length) {
    throw new Error(`Workflow sync failed for ${failures.length} repositories: ${failures.join(", ")}`);
  }
}

module.exports = { loadPlan, syncTarget, syncWorkflows };
