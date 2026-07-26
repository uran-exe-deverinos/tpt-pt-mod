# Builds The Powder Toy with the MSVC (Visual Studio) environment loaded.
# Usage from any PowerShell:  .\build.ps1              (builds the 'build' dir)
#                             .\build.ps1 build-debug   (builds another dir)
#                             .\build.ps1 build -run     (build, then launch powder.exe)
param(
    [string]$Dir = "build",
    [switch]$Run
)

$vsPath = "C:\Program Files\Microsoft Visual Studio\18\Community"
Import-Module "$vsPath\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath $vsPath -SkipAutomaticLocation -DevCmdArguments "-arch=x64 -host_arch=x64" | Out-Null

$buildDir = Join-Path $PSScriptRoot $Dir
ninja -C $buildDir
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if ($Run) { & (Join-Path $buildDir "powder.exe") }
