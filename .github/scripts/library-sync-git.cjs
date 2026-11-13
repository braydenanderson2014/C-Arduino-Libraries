const { execFileSync } = require("node:child_process");
const { getCredentials, withGitCredential, redactCredentials } = require("./library-sync-credentials.cjs");

try {
  const credentials = getCredentials();
  withGitCredential(credentials, token => {
    try {
      execFileSync("git", process.argv.slice(2), {
        stdio: ["ignore", "pipe", "pipe"],
        env: {
          ...process.env,
          GIT_TERMINAL_PROMPT: "0",
          GIT_CONFIG_COUNT: "1",
          GIT_CONFIG_KEY_0: "http.https://github.com/.extraheader",
          GIT_CONFIG_VALUE_0: `AUTHORIZATION: basic ${Buffer.from(`x-access-token:${token}`).toString("base64")}`
        }
      });
    } catch (error) {
      throw new Error(redactCredentials(error.message, credentials));
    }
  });
} catch (error) {
  console.error(error.message);
  process.exitCode = 1;
}
