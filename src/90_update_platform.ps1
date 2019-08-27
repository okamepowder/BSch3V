# copyright (c) 2019 okamepowder
#
# How to run:
#   powershell -ExecutionPolicy RemoteSigned -File .\90_update_platform.ps1
#

$PlatformToolset_D="v141"
$PlatformToolset_R="v141_xp" # 140_xp or 141_xp : 141_xp can be installed from Visual Studio Installer.
$pattern_D="(?si)('Debug\|Win32'"" Label=""Configuration"">\s*<ConfigurationType>[^<>]+</ConfigurationType>\s*<PlatformToolset>)[^><]+(</PlatformToolset>)"
$pattern_R="(?si)('Release\|Win32'"" Label=""Configuration"">\s*<ConfigurationType>[^<>]+</ConfigurationType>\s*<PlatformToolset>)[^><]+(</PlatformToolset>)"
$array = Get-ChildItem -Recurse -Name  -Filter *.vcxproj | Sort-Object
foreach ($fname in $array) { 
    $fileContent = Get-Content $fname -Raw
    #$s = [regex]::Replace($fileContent, "(?s)(\r\n|\n\r|\n|\r)", { "`n" })
    $s = [regex]::Replace($fileContent, "(\r\n|\n\r|\n|\r)", { "`r`n" })
    $s = [regex]::Replace($s, $pattern_D, { $args.groups[1].value + $PlatformToolset_D + $args.groups[2].value })
    $s = [regex]::Replace($s, $pattern_R, { $args.groups[1].value + $PlatformToolset_R + $args.groups[2].value })
    if ($fileContent -ne $s -and ($s.Length -gt 0) ) { # -gt: >
        Write-Output $fname
        Write-Output $s
        #[IO.File]::WriteAllText($fname, $s, [Text.Encoding]::UTF8)
        [IO.File]::WriteAllText($fname, $s, [System.Text.UTF8Encoding]$true)
    }
}
