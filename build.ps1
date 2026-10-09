$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location -LiteralPath $root
New-Item -ItemType Directory -Force -Path 'build' | Out-Null
$compiler = 'C:\msys64\ucrt64\bin\g++.exe'
foreach ($name in @('task1_5', 'task2_1', 'task2_2', 'task2_3', 'task3_1', 'task3_2', 'task3_3')) {
    $source = "src/$name.cpp"
    $output = "build/$name.exe"
    & $compiler -std=c++17 -Wall -Wextra -pedantic $source -o $output
    if ($LASTEXITCODE -ne 0) { throw "Build failed: $name" }
    Write-Host "Built $output"
}
Pop-Location
