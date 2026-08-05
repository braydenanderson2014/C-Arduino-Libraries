[CmdletBinding()]
param(
    [string]$BuildDir = "build/host_arduino_sim_cmake",
    [string]$Config = "Release",
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

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Set-Location $repoRoot

$cmakeExe = Resolve-CMakeExe
$ctestExe = Resolve-CTestExe
$buildDirPath = (Join-Path $repoRoot $BuildDir)

if ($Reconfigure -or -not (Test-Path $buildDirPath)) {
    Write-Host "[host-sim] Configuring CMake project in '$BuildDir'..."
    & $cmakeExe -S test/host_arduino_sim -B $BuildDir
}

Write-Host "[host-sim] Building targets (config: $Config)..."
& $cmakeExe --build $BuildDir --config $Config --target host_stress_off host_stress_on

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
& $ctestExe @ctestArgs

Write-Host "[host-sim] Done."
