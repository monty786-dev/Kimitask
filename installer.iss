#define MyAppName      "WinlatorShell"
#define MyAppVersion   "1.0.0"
#define MyAppPublisher "YourName"
#define MyAppExe       "winlatorshell.exe"

[Setup]
AppId={{8F3A2C11-9D4E-4B6A-A1C2-WINLAT0RSHE11}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=dist
OutputBaseFilename=WinlatorShell_Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
WizardSizePercent=110
DisableWelcomePage=no
SetupIconFile=assets\icons\setup.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop shortcut"; GroupDescription: "Shortcuts:"
Name: "autostart";   Description: "Launch WinlatorShell when the container boots"; GroupDescription: "Integration:"; Flags: checkedonce

[Files]
Source: "build\{#MyAppExe}"; DestDir: "{app}"; Flags: ignoreversion
Source: "assets\*";           DestDir: "{app}\assets"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExe}"; Tasks: desktopicon

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run";
ValueType: string; ValueName: "WinlatorShell";
ValueData: """{app}\{#MyAppExe}"""; Tasks: autostart

[Run]
Filename: "{app}\{#MyAppExe}"; Description: "Launch {#MyAppName}"; Flags: nowait postinstall skipifsilent
