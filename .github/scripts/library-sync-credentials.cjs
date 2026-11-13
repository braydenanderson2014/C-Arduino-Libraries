function getCredentials(env = process.env) {
  const credentials = [];
  for (const name of ["LIBRARY_SYNC_TOKEN", "GH_PROJECT_TOKEN"]) {
    const token = (env[name] || "").trim();
    if (token && !credentials.some(credential => credential.token === token)) {
      credentials.push({ name, token });
    }
  }
  if (!credentials.length) {
    throw new Error("Configure LIBRARY_SYNC_TOKEN or GH_PROJECT_TOKEN with Contents, Issues, and Pull requests read/write access to the monorepo and remote library repositories.");
  }
  return credentials;
}

function enableCredentialFallback(github, core, env = process.env) {
  const credentials = getCredentials(env);
  for (const { token } of credentials) core.setSecret(token);
  let preferred = credentials.find(credential => credential.token === env.SYNC_TOKEN) || credentials[0];
  github.hook.wrap("request", async (request, options) => {
    const candidates = [preferred, ...credentials.filter(credential => credential !== preferred)];
    let lastError;
    for (const credential of candidates) {
      try {
        const response = await request({
          ...options,
          headers: { ...options.headers, authorization: `token ${credential.token}` }
        });
        preferred = credential;
        return response;
      } catch (error) {
        // Retry only definitive access rejections, never ambiguous network/write failures.
        if (![401, 403, 404].includes(error.status)) throw error;
        lastError = error;
        core.warning(`${credential.name} could not authorize this API operation (HTTP ${error.status}); trying the next credential.`);
      }
    }
    core.error("Neither library sync credential can authorize the requested operation. Renew the secrets and grant the required repository read/write permissions.");
    throw lastError;
  });
  return () => preferred.token;
}

function withGitCredential(credentials, operation) {
  let lastError;
  for (const { name, token } of credentials) {
    try {
      return operation(token);
    } catch (error) {
      if (!/Authentication failed|Invalid username or token|Repository not found|requested URL returned error: (401|403)|Permission to .+ denied|Write access to repository not granted/i.test(error.message)) {
        throw error;
      }
      lastError = error;
      console.warn(`${name} could not authorize this Git operation; trying the next credential.`);
    }
  }
  throw lastError;
}

function redactCredentials(value, credentials) {
  let result = String(value || "");
  for (const { token } of credentials) {
    result = result.split(token).join("***").split(encodeURIComponent(token)).join("***");
  }
  return result;
}

module.exports = { getCredentials, enableCredentialFallback, withGitCredential, redactCredentials };
