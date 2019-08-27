# copyright (c) 2019 okamepowder
#
# How to run:
#   powershell -ExecutionPolicy RemoteSigned -File .\90_update_switch_mfc.ps1 [static|dynamic]
#   default mode static
#

Param([String]$mode = "static")

if ($mode -ne "dynamic" ) {
    $UseOfMfc="Static"
    $RuntimeLibraryD="MultiThreadedDebug"
    $RuntimeLibraryR="MultiThreaded"
 } else {
    $UseOfMfc="Dynamic"
    $RuntimeLibraryD="MultiThreadedDebugDLL"
    $RuntimeLibraryR="MultiThreadedDLL"
}
$pattern_U="(?si)(<UseOfMfc>)[^<>]+(</UseOfMfc>)"
$pattern_D="(?si)(<ItemDefinitionGroup [^<>]+'Debug[^<>]+>.+?<RuntimeLibrary>)[^<>]+(</RuntimeLibrary>)"
$pattern_R="(?si)(<ItemDefinitionGroup [^<>]+'Release[^<>]+>.+?<RuntimeLibrary>)[^<>]+(</RuntimeLibrary>)"
$array = Get-ChildItem -Recurse -Name  -Filter *.vcxproj | Sort-Object
foreach ($fname in $array) { 
    $fileContent = Get-Content $fname -Raw
    $s = [regex]::Replace($fileContent, "(\r\n|\n\r|\n|\r)", { "`r`n" })
    $s = [regex]::Replace($s, $pattern_U, { $args.groups[1].value + $UseOfMfc + $args.groups[2].value })
    $s = [regex]::Replace($s, $pattern_D, { $args.groups[1].value + $RuntimeLibraryD + $args.groups[2].value })
    $s = [regex]::Replace($s, $pattern_R, { $args.groups[1].value + $RuntimeLibraryR + $args.groups[2].value })
    if ($fileContent -ne $s -and ($s.Length -gt 0) ) { # -gt: >
        Write-Output $fname
        Write-Output $s
        [IO.File]::WriteAllText($fname, $s, [System.Text.UTF8Encoding]$true) # with bom
    }
}
