[CmdletBinding()]
param(
    [string]$BuildDir = "build/host_arduino_sim_cmake",
    [string]$Config = "Release",
    [string]$Generator,
    [string]$ArtifactsDir = "test/host_arduino_sim/out",
    [string]$OutputMarkdown,
    [string]$OutputJson,
    [string]$TestRegex,
    [switch]$StressOnly,
    [switch]$VerboseCtest,
    [switch]$Reconfigure
)

$ErrorActionPreference = "Stop"

function Resolve-CMakeExe {
    $cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmakeCmd) {
        return $cmakeCmd.Source
    }

    $fallback = "C:\Program Files\CMake\bin\cmake.exe"
    if (Test-Path $fallback) {
        return $fallback
    }

    throw "Could not find cmake on PATH or at '$fallback'."
}

function Resolve-CTestExe {
    $ctestCmd = Get-Command ctest -ErrorAction SilentlyContinue
    if ($ctestCmd) {
        return $ctestCmd.Source
    }

    $fallback = "C:\Program Files\CMake\bin\ctest.exe"
    if (Test-Path $fallback) {
        return $fallback
    }

    throw "Could not find ctest on PATH or at '$fallback'."
}

function Resolve-PythonExe {
    $pythonCmd = Get-Command python -ErrorAction SilentlyContinue
    if ($pythonCmd) {
        return $pythonCmd.Source
    }

    $pyCmd = Get-Command py -ErrorAction SilentlyContinue
    if ($pyCmd) {
        return $pyCmd.Source
    }

    throw "Could not find python or py on PATH. Python is required to generate the consolidated host-sim report."
}

function Invoke-Native {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Exe,
        [Parameter(Mandatory = $true)]
        [string[]]$Args,
        [Parameter(Mandatory = $true)]
        [string]$Step
    )

    & $Exe @Args
    if ($LASTEXITCODE -ne 0) {
        throw "$Step failed with exit code $LASTEXITCODE."
    }
}

function Resolve-PreferredGenerator {
    param(
        [string]$RequestedGenerator
    )

    if ($RequestedGenerator) {
        return $RequestedGenerator
    }

    if (Get-Command ninja -ErrorAction SilentlyContinue) {
        return "Ninja"
    }

    if (Get-Command mingw32-make -ErrorAction SilentlyContinue) {
        return "MinGW Makefiles"
    }

    if (Get-Command nmake -ErrorAction SilentlyContinue) {
        return "NMake Makefiles"
    }

    if (Get-Command make -ErrorAction SilentlyContinue) {
        return "Unix Makefiles"
    }

    throw "No supported CMake generator tool was found on PATH (ninja, mingw32-make, nmake, or make). Install build tools or pass -Generator explicitly."
}

function Get-CMakeCacheValue {
    param(
        [Parameter(Mandatory = $true)]
        [string]$CachePath,
        [Parameter(Mandatory = $true)]
        [string]$Key
    )

    if (-not (Test-Path $CachePath)) {
        return $null
    }

    $pattern = "^{0}(:[^=]+)?=(.*)$" -f [regex]::Escape($Key)
    $line = Select-String -Path $CachePath -Pattern $pattern | Select-Object -First 1
    if (-not $line) {
        return $null
    }

    return $line.Matches[0].Groups[2].Value.Trim()
}

function Get-ArtifactFiles {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ArtifactsRoot,
        [Parameter(Mandatory = $true)]
        [string]$Filter
    )

    if (-not (Test-Path $ArtifactsRoot)) {
        return @()
    }

    return @(Get-ChildItem -Path $ArtifactsRoot -Filter $Filter -File -Recurse)
}

function Remove-ArtifactFiles {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ArtifactsRoot,
        [Parameter(Mandatory = $true)]
        [string[]]$Filters
    )

    if (-not (Test-Path $ArtifactsRoot)) {
        return
    }

    foreach ($filter in $Filters) {
        $matches = Get-ArtifactFiles -ArtifactsRoot $ArtifactsRoot -Filter $filter
        foreach ($match in $matches) {
            Remove-Item -Path $match.FullName -Force -ErrorAction SilentlyContinue
        }
    }
}

function Assert-ArtifactCount {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ArtifactsRoot,
        [Parameter(Mandatory = $true)]
        [string]$Filter,
        [Parameter(Mandatory = $true)]
        [int]$MinimumCount,
        [Parameter(Mandatory = $true)]
        [string]$Description
    )

    $matches = Get-ArtifactFiles -ArtifactsRoot $ArtifactsRoot -Filter $Filter
    if ($matches.Count -lt $MinimumCount) {
        throw "Expected at least $MinimumCount $Description artifact(s) matching '$Filter' under '$ArtifactsRoot', found $($matches.Count)."
    }

    return $matches
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Set-Location $repoRoot

$cmakeExe = Resolve-CMakeExe
$ctestExe = Resolve-CTestExe
$pythonExe = Resolve-PythonExe
$buildDirPath = (Join-Path $repoRoot $BuildDir)
$expectedSourceDir = (Resolve-Path (Join-Path $repoRoot "test/host_arduino_sim")).Path
$cachePath = Join-Path $buildDirPath "CMakeCache.txt"
$ctestFilePath = Join-Path $buildDirPath "CTestTestfile.cmake"
$artifactsDirPath = (Join-Path $repoRoot $ArtifactsDir)
$defaultReportStem = if ($StressOnly) { "host-sim-local-stress-report" } else { "host-sim-local-report" }
$outputMarkdownPath = if ($OutputMarkdown) { (Join-Path $repoRoot $OutputMarkdown) } else { (Join-Path $repoRoot "reports/$defaultReportStem.md") }
$outputJsonPath = if ($OutputJson) { (Join-Path $repoRoot $OutputJson) } else { (Join-Path $repoRoot "reports/$defaultReportStem.json") }
$isFilteredRun = -not [string]::IsNullOrWhiteSpace($TestRegex)

$needsConfigure = $Reconfigure -or -not (Test-Path $cachePath)

if (-not $needsConfigure) {
    $cachedSourceDir = Get-CMakeCacheValue -CachePath $cachePath -Key "CMAKE_HOME_DIRECTORY"
    if ($cachedSourceDir -and (([System.IO.Path]::GetFullPath($cachedSourceDir)) -ne ([System.IO.Path]::GetFullPath($expectedSourceDir)))) {
        Write-Host "[host-sim] Cached source dir '$cachedSourceDir' does not match expected '$expectedSourceDir'. Reconfiguring..."
        $needsConfigure = $true
    }
}

if (-not $needsConfigure -and -not (Test-Path $ctestFilePath)) {
    Write-Host "[host-sim] Missing CTest metadata in '$BuildDir'. Reconfiguring..."
    $needsConfigure = $true
}

if ($needsConfigure -and (Test-Path $buildDirPath)) {
    # Clear stale cache so generator and source directory changes are applied cleanly.
    Remove-Item -Path (Join-Path $buildDirPath "CMakeCache.txt") -ErrorAction SilentlyContinue
    Remove-Item -Path (Join-Path $buildDirPath "CMakeFiles") -Recurse -Force -ErrorAction SilentlyContinue
}

if ($needsConfigure) {
    $resolvedGenerator = Resolve-PreferredGenerator -RequestedGenerator $Generator
    Write-Host "[host-sim] Configuring CMake project in '$BuildDir'..."
    $configureArgs = @(
        "-S", "test/host_arduino_sim",
        "-B", $BuildDir,
        "-G", $resolvedGenerator,
        "-DBUILD_TESTING=ON"
    )
    Invoke-Native -Exe $cmakeExe -Args $configureArgs -Step "CMake configure"
}

$discoverArgs = @("--test-dir", $BuildDir, "-N")
Write-Host "[host-sim] Verifying CTest discovery..."
Invoke-Native -Exe $ctestExe -Args $discoverArgs -Step "CTest discovery"

$buildTargets = if ($StressOnly) {
    @("host_stress_off", "host_stress_on")
} else {
    @("host_sim_all")
}

if ($StressOnly) {
    Remove-ArtifactFiles -ArtifactsRoot $artifactsDirPath -Filters @("stress-*.json")
} else {
    Remove-ArtifactFiles -ArtifactsRoot $artifactsDirPath -Filters @("report-*.json", "report-json-*.json", "stats-*.json", "stress-*.json")
}

if (Test-Path $outputMarkdownPath) {
    Remove-Item -Path $outputMarkdownPath -Force -ErrorAction SilentlyContinue
}
if (Test-Path $outputJsonPath) {
    Remove-Item -Path $outputJsonPath -Force -ErrorAction SilentlyContinue
}

Write-Host "[host-sim] Building targets (config: $Config): $($buildTargets -join ', ')"
$buildArgs = @("--build", $BuildDir, "--config", $Config, "--target") + $buildTargets
Invoke-Native -Exe $cmakeExe -Args $buildArgs -Step "CMake build"

$ctestArgs = @("--test-dir", $BuildDir, "-C", $Config)

if ($VerboseCtest) {
    $ctestArgs += "-V"
} else {
    $ctestArgs += "--output-on-failure"
}

if ($StressOnly) {
    $ctestArgs += @("-L", "stress")
}

if ($TestRegex) {
    $ctestArgs += @("-R", $TestRegex)
}

Write-Host "[host-sim] Running CTest with args: $($ctestArgs -join ' ')"
Invoke-Native -Exe $ctestExe -Args $ctestArgs -Step "CTest run"

if ($StressOnly) {
    Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "stress-*.json" -MinimumCount 1 -Description "stress report"
    $reportMode = "stress"
} else {
    if ($isFilteredRun) {
        Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "*.json" -MinimumCount 1 -Description "host-sim JSON"
    } else {
        Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "report-*.json" -MinimumCount 4 -Description "integration report"
        Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "stats-*.json" -MinimumCount 4 -Description "per-test stats"
        Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "report-json-*.json" -MinimumCount 4 -Description "JSON mode report"
        Assert-ArtifactCount -ArtifactsRoot $artifactsDirPath -Filter "stress-*.json" -MinimumCount 4 -Description "stress report"
    }
    $reportMode = "standard"
}

$reportArgs = @(
    (Join-Path $repoRoot "scripts/generate_host_sim_report.py"),
    "--mode", $reportMode,
    "--artifacts-dir", $artifactsDirPath,
    "--output-md", $outputMarkdownPath,
    "--output-json", $outputJsonPath
)

Write-Host "[host-sim] Generating consolidated report..."
Invoke-Native -Exe $pythonExe -Args $reportArgs -Step "Host-sim report generation"

if (-not (Test-Path $outputMarkdownPath) -or -not (Test-Path $outputJsonPath)) {
    throw "Consolidated host-sim report generation did not produce expected outputs '$outputMarkdownPath' and '$outputJsonPath'."
}

Write-Host "[host-sim] Markdown report: $outputMarkdownPath"
Write-Host "[host-sim] JSON report:     $outputJsonPath"

Write-Host "[host-sim] Done."
