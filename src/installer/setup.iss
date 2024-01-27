﻿; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

; innosetup 5.6(unicode版) : XP対応
; (保留) innosetup 6.0 : vista以降
; Todo: インストール時: .iniの保存先
; Todo: アンインストール時: .iniの扱い [残す or 削除]
; Todo: インストール先 - 共通フォルダ、ユーザーフォルダの選択

#define MyAppName "回路図作成アプリ"
#define MyAppVersion "0.84.0.0"
#define MyAppURL "https://github.com/okamepowder/BSch3V"
#define MyAppExeName "bsch3v.exe"
#define MyAppSetupFileName "回路図作成アプリ-setup-" + "2024-01-26_0.84"

#define MyMaterialRootPath "..\..\"
#define MyMaterialReleasePath "..\..\src\Release\"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)

AppId="{{2F5A2AF6-1A07-4F90-9C60-D5A7DF5FF20B}"
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\circuit\bsch3v
DisableDirPage=yes
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile="{#MyMaterialRootPath}License\LICENSE.txt"
OutputDir=.\build
OutputBaseFilename="{#MyAppSetupFileName}"
Compression=lzma
SolidCompression=yes
PrivilegesRequired=none

[Languages]
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "{#MyMaterialReleasePath}bsch3v.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}CE3Search.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}LCoV.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}nl3w.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}nl3wpads.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}nut3w.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialReleasePath}pl3w.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialRootPath}src\template\BSCH3.INI"; DestDir: "{app}"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}README.html"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialRootPath}history.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyMaterialRootPath}license\license.txt"; DestDir: "{app}"; Flags: ignoreversion
; lib
Source: "{#MyMaterialRootPath}lib\ANALOG.LB3"; DestDir: "{app}\lib"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}lib\CONSW.LB3"; DestDir: "{app}\lib"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}lib\DIGITAL.LB3"; DestDir: "{app}\lib"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}lib\DISCRETE.LB3"; DestDir: "{app}\lib"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}lib\LOGIC74.LB3"; DestDir: "{app}\lib"; Flags: onlyifdoesntexist
; sample
Source: "{#MyMaterialRootPath}sample\LEDPORT.CE3"; DestDir: "{app}\sample"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}sample\Radio.CE3"; DestDir: "{app}\sample"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}sample\SensorIF.CE3"; DestDir: "{app}\sample"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}sample\コメントサンプル.CE3"; DestDir: "{app}\sample"; Flags: onlyifdoesntexist
Source: "{#MyMaterialRootPath}sample\NL3W\ERRREPORT.CE3"; DestDir: "{app}\sample\NL3W"; Flags: onlyifdoesntexist



; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
;Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\回路図エディタ（bsch3v）"; Filename: "{app}\bsch3v.exe"
Name: "{group}\部品ライブラリエディタ（lcov）"; Filename: "{app}\lcov.exe"
Name: "{group}\部品リスト作成（pl3w）"; Filename: "{app}\pl3w.exe"
Name: "{group}\ネットリスト作成（nl3w）"; Filename: "{app}\nl3w.exe"
Name: "{group}\部品番号振り付け（nut3w）"; Filename: "{app}\nut3w.exe"
Name: "{group}\回路図ファイル用の検索（CE3Search）"; Filename: "{app}\CE3Search.exe"
Name: "{group}\はじめにお読みください"; Filename: "{app}\README.html"
;
Name: "{group}\アンインストール（{#MyAppName}）"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[UninstallDelete]
Type: files; Name: "{app}\bsch3v.ini"
Type: files; Name: "{app}\CE3Search.ini"
Type: files; Name: "{app}\LCoV.ini"
Type: files; Name: "{app}\nl3w.ini"
Type: files; Name: "{app}\nut3w.ini"
Type: files; Name: "{app}\pl3w.ini"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Registry]
;Root: HKCU; Subkey: "SOFTWARE\名前"; Flags: uninsdeletekeyifempty dontcreatekey
