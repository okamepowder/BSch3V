# copyright (c) 2019 okamepowder
#
# How to run:
#   powershell -ExecutionPolicy RemoteSigned -File .\90_update_winver.ps1
#

$winver="0x0501"
$iever="0x0600"
# Windows xp: win 0x0501 , ie 0x600
# Windows  7: win 0x0601 , ie 0x800
# Windows  8: win 0x0602 , ie 0xA00

$cmt_win0='// Windows XP 以降のバージョンに固有の機能の使用を許可します。'
$cmt_win1='// これを Windows XP またはそれ以降のバージョン向けに適切な値に変更してください。'
$cmt_win9x0='// Windows ME 以降のバージョンに固有の機能の使用を許可します。'
$cmt_win9x1='// これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。'
$cmt_ie0='// IE 6.0 以降のバージョンに固有の機能の使用を許可します。'
$cmt_ie1='// これを IE 6.0  またはそれ以降のバージョン向けに適切な値に変更してください。'

$array = Get-ChildItem -Recurse -Name  -include targetver.h,stdafx.h | Sort-Object
foreach ($fname in $array) { 
    $fileContent = Get-Content $fname -Raw
    $s = [regex]::Replace($fileContent, "(?s)(\r\n|\n\r|\n|\r)", { "`r`n" })
    $s = [regex]::Replace($s, "(?m)(#ifndef\s+(?:WINVER|_WIN32_WINNT))\s+//.+$", { ($args.groups[1].value).PadRight(32) + $cmt_win0 })
    $s = [regex]::Replace($s, "(?m)(#ifndef\s+_WIN32_WINDOWS)\s+//.+$", { ($args.groups[1].value).PadRight(32) + $cmt_win9x0 })
    $s = [regex]::Replace($s, "(?m)(#ifndef\s+_WIN32_IE)\s+//.+$", { ($args.groups[1].value).PadRight(32) + $cmt_ie0 })
    $s = [regex]::Replace($s, "(?m)(#define\s+(?:WINVER|_WIN32_WINNT))\s+0x\d+\s*.*$", { ($args.groups[1].value + " " + $winver).PadRight(32) + $cmt_win1 })
    $s = [regex]::Replace($s, "(?m)(#define\s+_WIN32_IE)\s+0x\d+\s*.*$", { ($args.groups[1].value + " " + $iever).PadRight(32) + $cmt_ie1 })
    $s = [regex]::Replace($s, "(?m)(#define\s+_WIN32_WINDOWS)\s+0x\d+\s*.*$", { ($args.groups[1].value + " " + "0x0500").PadRight(32) + $cmt_win9x1 })
    if ($fileContent -ne $s -and ($s.Length -gt 0) ) { # -gt: >
        Write-Output $fname
        Write-Output $s
        [IO.File]::WriteAllText($fname, $s, [System.Text.Encoding]::GetEncoding(932)) # Shift_JIS
        #[IO.File]::WriteAllText($fname, $s, [System.Text.UTF8Encoding]$false) # UTF8 without bom
        #[IO.File]::WriteAllText($fname, $s, [System.Text.UTF8Encoding]$true)  # UTF8 with bom
    }
}
