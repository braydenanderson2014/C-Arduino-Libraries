[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [switch]$IncludeBuild,
    [switch]$IncludeOutBin,
    [switch]$IncludeReports,
    [switch]$All
)

$ErrorActionPreference = "Stop"

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Set-Location $repoRoot

if ($All) {
    $IncludeBuild = $true
    $IncludeOutBin = $true
    $IncludeReports = $true
}

# Default behavior if no switches are supplied.
if (-not $IncludeBuild -and -not $IncludeOutBin -and -not $IncludeReports) {
    $IncludeOutBin = $true
    $IncludeReports = $true
}

$pathsToRemove = @()
if ($IncludeBuild) {
    $pathsToRemove += "build/host_arduino_sim_cmake"
}
if ($IncludeOutBin) {
    $pathsToRemove += "test/host_arduino_sim/out/bin"
}

foreach ($path in $pathsToRemove) {
    if (Test-Path $path) {
        if ($PSCmdlet.ShouldProcess($path, "Remove directory")) {
            Remove-Item -Path $path -Recurse -Force
            Write-Host "Removed: $path"
        }
    } else {
        Write-Host "Skipped (not found): $path"
    }
}

if ($IncludeReports) {
    $reportPatterns = @(
        "test/host_arduino_sim/out/stress-*.json",
        "test/host_arduino_sim/out/report-*.json",
        "test/host_arduino_sim/out/stats-*.json"
    )

    foreach ($pattern in $reportPatterns) {
        $files = Get-ChildItem -Path $pattern -File -ErrorAction SilentlyContinue
        foreach ($file in $files) {
            if ($PSCmdlet.ShouldProcess($file.FullName, "Remove file")) {
                Remove-Item -Path $file.FullName -Force
                Write-Host "Removed: $($file.FullName)"
            }
        }
    }
}

Write-Host "Cleanup complete."
