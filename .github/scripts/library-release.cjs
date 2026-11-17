const fs = require("node:fs");
const path = require("node:path");

function parseSections(body) {
  const sections = {};
  let heading;
  for (const line of String(body || "").replace(/\r/g, "").split("\n")) {
    const match = /^### (.+)$/.exec(line);
    if (match) {
      heading = match[1].trim();
      sections[heading] = [];
    } else if (heading) {
      sections[heading].push(line);
    }
  }
  return Object.fromEntries(Object.entries(sections).map(([name, lines]) => {
    const value = lines.join("\n").trim();
    return [name, value === "_No response_" ? "" : value];
  }));
}

function resolveRequest(config, body) {
  const sections = parseSections(body);
  const library = sections.Library;
  const target = config.libraries?.[library];
  if (!target || target.enabled === false || !target.targetRepo) {
    throw new Error("Select an enabled library in .github/library-sync-targets.json.");
  }
  const version = sections.Version || "";
  if (!/^\d+\.\d+\.\d+(?:-[0-9A-Za-z-]+(?:\.[0-9A-Za-z-]+)*)?$/.test(version)) {
    throw new Error("Version must be a semantic version without a v prefix (for example 1.2.3).");
  }
  const owner = target.targetOwner || config.defaults?.targetOwner;
  if (!/^[A-Za-z0-9-]+$/.test(owner || "") || !/^[A-Za-z0-9_.-]+$/.test(target.targetRepo)) {
    throw new Error("Invalid configured remote repository.");
  }
  return {
    library, version, owner, repo: target.targetRepo,
    branch: target.baseBranch || config.defaults?.baseBranch || "main",
    prerelease: version.includes("-"),
    notes: sections["Release notes"] || ""
  };
}

function changelogNotes(report) {
  return /^## Next Release[^\n]*\n([\s\S]*?)(?=^\| Date|^## |$(?![\s\S]))/m.exec(report)?.[1].trim() || "";
}

async function requestRelease({ github, context, core, request, root = process.env.GITHUB_WORKSPACE }) {
  const issueNumber = context.eventName === "workflow_dispatch"
    ? Number(String(context.payload.inputs.issue_number).replace(/^#/, ""))
    : context.payload.issue.number;
  if (!Number.isSafeInteger(issueNumber) || issueNumber <= 0) throw new Error("Invalid issue number.");
  const source = { ...context.repo, issue_number: issueNumber };
  const { data: issue } = await github.rest.issues.get(source);
  if (issue.pull_request || !issue.labels.some(label => label.name === "library-release-request")) {
    throw new Error("Expected a library-release-request issue.");
  }
  const users = new Set([issue.user.login, context.actor]);
  for (const username of users) {
    const { data } = await github.rest.repos.getCollaboratorPermissionLevel({ ...context.repo, username });
    if (!["admin", "maintain"].includes(data.permission)) {
      await github.rest.issues.createComment({ ...source, body: "Release requests are restricted to collaborators with maintain or admin permission. No release was dispatched." });
      throw new Error(`Release permission denied for ${username}.`);
    }
  }
  const config = JSON.parse(fs.readFileSync(path.join(root, ".github/library-sync-targets.json"), "utf8"));
  const release = resolveRequest(config, issue.body);
  if (!release.notes) {
    const report = path.join(root, "reports/library-changelogs", `${release.library}.md`);
    if (fs.existsSync(report)) release.notes = changelogNotes(fs.readFileSync(report, "utf8"));
  }
  if (Buffer.byteLength(release.notes, "utf8") > 30000) throw new Error("Release notes exceed 30 KB.");
  await request(client => client.rest.repos.createDispatchEvent({
    owner: release.owner, repo: release.repo, event_type: "library-release-request",
    client_payload: {
      library: release.library, version: release.version, branch: release.branch,
      prerelease: release.prerelease, notes: release.notes, source_issue: issue.html_url
    }
  }));
  await github.rest.issues.createComment({
    ...source,
    body: `Release v${release.version} requested in **${release.owner}/${release.repo}** from \`${release.branch}\`.\n\n[Remote build runs](https://github.com/${release.owner}/${release.repo}/actions/workflows/build-library-release.yml)\n\n${release.notes || "Release notes will be generated from the remote repository's release history."}`
  });
  core.info("Remote release requested; consult the remote build run for the publication result.");
}

module.exports = { parseSections, resolveRequest, changelogNotes, requestRelease };