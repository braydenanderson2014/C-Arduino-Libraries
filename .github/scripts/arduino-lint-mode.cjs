const fs = require("node:fs");
const path = require("node:path");

function libraryManagerMode({ root, repository, libraryDirectory, config = {} }) {
  if (!config || typeof config !== "object" || Array.isArray(config)) {
    throw new Error("Arduino lint configuration must be a JSON object.");
  }
  const exceptions = config.skipLibraryManager ?? {};
  if (!exceptions || typeof exceptions !== "object" || Array.isArray(exceptions)) {
    throw new Error("skipLibraryManager must map owner/repository names to arrays of library paths.");
  }
  for (const [name, directories] of Object.entries(exceptions)) {
    if (!/^[A-Za-z0-9-]+\/[A-Za-z0-9_.-]+$/.test(name) || !Array.isArray(directories) ||
        directories.some(directory => typeof directory !== "string" ||
          (directory !== "." && (!directory || directory.includes("\\") || directory.includes(":") ||
            directory.split("/").some(part => !part || part === "." || part === ".."))))) {
      throw new Error(`Invalid Library Manager exception for ${name}; use repository-relative paths or '.' for the root library.`);
    }
  }
  const relative = path.relative(root, path.resolve(root, libraryDirectory));
  if (relative === ".." || relative.startsWith(`..${path.sep}`) || path.isAbsolute(relative)) {
    throw new Error("The library directory must be inside the repository.");
  }
  const libraryPath = relative.split(path.sep).join("/") || ".";
  const directories = Object.entries(exceptions)
    .find(([name]) => name.toLowerCase() === String(repository || "").toLowerCase())?.[1] || [];
  return directories.includes(libraryPath) ? "false" : "update";
}

if (require.main === module) {
  try {
    if (!process.argv[2]) throw new Error("A library directory is required.");
    const root = process.cwd();
    const configPath = path.join(root, ".github/arduino-lint-config.json");
    const config = fs.existsSync(configPath) ? JSON.parse(fs.readFileSync(configPath, "utf8")) : {};
    process.stdout.write(libraryManagerMode({
      root, repository: process.env.GITHUB_REPOSITORY, libraryDirectory: process.argv[2], config
    }));
  } catch (error) {
    process.stderr.write(`Arduino lint configuration error: ${error.message}\n`);
    process.exitCode = 1;
  }
}

module.exports = { libraryManagerMode };