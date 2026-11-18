function isAutomationIssue(issue) {
  const author = issue?.user;
  const body = issue?.body || "";
  return author?.type === "Bot" ||
    (author?.login || "").toLowerCase().endsWith("[bot]") ||
    /<!--\s*(?:mirror-source:|auto-library-sync-request\s*-->)/i.test(body);
}

module.exports = { isAutomationIssue };
