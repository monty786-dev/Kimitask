#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <windows.h>
#include <wchar.h>
#include <stdbool.h>
#include <stdint.h>

#define WS_MAX_SHORTCUTS   256
#define WS_MAX_APPS        512
#define WS_MAX_TRAY_APPS   64
#define WS_MAX_EXC_APPS    128
#define WS_MAX_NAME        128
#define WS_MAX_PATH        MAX_PATH

/* ---------- Enumerations ---------- */

typedef enum {
    THEME_NONE = 0,
    THEME_WIN98, THEME_WIN7, THEME_WIN8, THEME_WIN10,
    THEME_UBUNTU, THEME_ARCH, THEME_MINT, THEME_XFCE,
    THEME_CINNAMON, THEME_KDE,
    THEME_COUNT
} WsTheme;

typedef enum { MODE_CLASSIC_OG = 0, MODE_MODERN } WsThemeMode;

typedef enum { TBPOS_BOTTOM = 0, TBPOS_TOP, TBPOS_LEFT, TBPOS_RIGHT } WsTaskbarPos;
typedef enum { TBALIGN_LEFT = 0, TBALIGN_CENTER, TBALIGN_RIGHT } WsTaskbarAlign;

typedef enum {
    TBICONS_ICON_ONLY = 0, TBICONS_ICON_TITLE
} WsRunningIconMode;

typedef enum { DIV_THIN = 0, DIV_MID, DIV_BOLD } WsDividerStyle;

typedef enum { FIT_CROP = 0, FIT_FIT, FIT_STRETCH } WsWallpaperFit;

typedef enum { TB_FULLSCREEN_KEEP = 0, TB_FULLSCREEN_OVER,
               TB_FULLSCREEN_FLOAT_ICON, TB_FULLSCREEN_AUTOHIDE
} WsFullscreenMode;

typedef enum { APPLY_FULLSCREEN = 0, APPLY_MAXIMIZED, APPLY_BOTH } WsApplyScope;

typedef enum { ANIM_BLINK = 0, ANIM_POPUP, ANIM_SLIDE_LEFT,
               ANIM_SLIDE_RIGHT, ANIM_SLIDE_BOTTOM
} WsRevealAnim;

typedef enum { PANEL_NORMAL = 0, PANEL_MID, PANEL_LARGE, PANEL_FULLSCREEN
} WsPanelSize;

typedef enum { COLORMODE_NONE = 0, COLORMODE_DARK, COLORMODE_LIGHT,
               COLORMODE_AMOLED, COLORMODE_ROSE, COLORMODE_SAPPHIRE,
               COLORMODE_AERO
} WsColorMode;

typedef enum {
    CUR_NORMAL = 0, CUR_WAIT, CUR_WORKING, CUR_LINK, CUR_GRAB,
    CUR_IBEAM, CUR_CROSS, CUR_HAND, CUR_MOVE, CUR_CURSORS_COUNT
} WsCursorState;

typedef enum { APPLY_TITLEBAR_ONLY = 0, APPLY_WHOLE_WINDOW } WsSkinScope;

/* ---------- Composite types ---------- */

typedef struct {           /* one entry in a shortcut collection */
    WCHAR  target[WS_MAX_PATH];
    WCHAR  args[256];
    WCHAR  iconPath[WS_MAX_PATH];
    int    iconIndex;
    WCHAR  displayName[WS_MAX_NAME];
    bool   pinned;
    int    order;          /* drag-reorder position          */
} WsShortcut;

typedef struct {
    WsShortcut items[WS_MAX_SHORTCUTS];
    int        count;
} WsShortcutList;

/* ---------- Taskbar ---------- */

typedef struct {
    WsTaskbarPos   position;
    WsTaskbarAlign alignment;        /* right-align flips tray/clock to left,
                                        start button+panel to right corner   */
    int            height;           /* slider */
    int            iconSize;         /* slider, px */
    int            edgeGap;          /* slider: floating dock detach, px */
    int            underlineHeight;  /* slider, px */
    bool           autoFitWidth;
    bool           showTitles;
    bool           roundedCorners;
    bool           underlineIndicator;
    bool           underlineFromActiveIcon; /* dynamic color extraction */
    WsDividerStyle dividerStyle;
    WsRunningIconMode runningIconMode;
    bool           previewEnabled;
    int            previewW;         /* slider */
    int            previewH;         /* slider */
    bool           showClockDay;
    COLORREF       bgColor;
    WsFullscreenMode fullscreenMode;
    WsApplyScope   applyScope;
    int            startupDelaySec;  /* HKCU Run delay */
    bool           autostartOnBoot;
} WsTaskbarCfg;

/* ---------- Reveal bar (edge trigger) ---------- */

typedef struct {
    bool     enabled;
    COLORREF barColor;
    bool     posTop;                 /* false = bottom */
    int      alignment;              /* 0 left / 1 mid / 2 right */
    bool     lShapeEnabled;
    int      lShapeW, lShapeH;       /* sliders */
    int      opacityPct;             /* slider 10..100 */
    int      height;                 /* slider */
    int      width;                  /* slider, up to screen width */
    int      visibleDurationSec;     /* slider */
    WsRevealAnim anim;
    WCHAR    exceptions[WS_MAX_EXC_APPS][WS_MAX_NAME]; /* app exception list */
    int      exceptionCount;
} WsRevealBarCfg;

/* ---------- Start panel ---------- */

typedef struct {
    WsPanelSize panelSize;
    WCHAR  startBtnIcon[WS_MAX_PATH];   /* custom icon, empty = theme default */
    COLORREF startBtnColor;
    bool   startBtnUseCustomColor;
    WCHAR  userPic[WS_MAX_PATH];
    WCHAR  userName[WS_MAX_NAME];
    bool   sidebarCustomBg;
    WCHAR  sidebarBgImage[WS_MAX_PATH];
    COLORREF sidebarBgColor;
    int    sidebarAlpha;             /* transparency slider */
    bool   tileCustomColors;
    int    tileAlpha;                /* transparency slider */
    bool   tileAnimEnabled;          /* box / slide rearrange anim */
    int    tileAnimStyle;            /* 0 box, 1 slide */
    int    tileSizePx;               /* global pinned-tile slider */
} WsStartPanelCfg;

/* ---------- Cursor engine ---------- */

typedef struct {
    bool    enabled;
    WCHAR   files[CUR_CURSORS_COUNT][WS_MAX_PATH]; /* .cur/.ani/.png/.ico */
    COLORREF tintColor;
    bool    tintEnabled;
    int     borderThickness;         /* px */
    COLORREF borderColor;
} WsCursorCfg;

/* ---------- Soft-keyboard auto-trigger ---------- */

typedef struct {
    bool detectIBeam;                /* IDC_IBEAM cursor change */
    bool detectCaretBlink;           /* vertical caret via GetGUIThreadInfo */
    bool detectClickInEdit;          /* left-click inside text fields */
    int  retriggerCooldownMs;
    WCHAR winlatorIpcKey[64];        /* Winlator IPC / virtual-key bridge */
} WsKeyboardCfg;

/* ---------- Window skinning ---------- */

typedef struct {
    bool    enabled;
    int     transparencyPct;         /* slider */
    WsSkinScope scope;               /* title bar only / entire window */
    bool    skinBorders;
} WsSkinningCfg;

/* ---------- Personalization & visual effects ---------- */

typedef struct {
    WsTheme     theme;
    WsThemeMode mode;
    WsColorMode colorMode;
    bool        fxGlass;
    bool        fxGaussianBlur;
    int         blurIntensity;       /* slider */
    bool        fxBevel3D;
    int         glassShine;          /* slider */
    int         glassGloss;          /* slider */
    bool        dropShadows;
    bool        roundedCornersGlobal;
    COLORREF    accentColor;
    /* granular per-surface application */
    bool fxOnTaskbar, fxOnStartPanel, fxOnPreviewBox, fxOnTitleBars, fxOnApps;
    int  iconPreset;                 /* index into 10+ built-in presets */
    WCHAR fontFace[LF_FACESIZE];
    int  fontSize;
    int  uiScalePct;                 /* global UI scale slider */
} WsVisualCfg;

/* ---------- Full Desktop Mode ---------- */

typedef struct {
    bool     enabled;
    int      gridSize;               /* px */
    bool     snapToGrid;
    int      cornerV;                /* 0 top, 1 bottom */
    int      cornerH;                /* 0 left, 1 right */
    bool     showIcons;
    bool     widgetClock, widgetLauncher;
    bool     useWallpaperImage;
    WCHAR    wallpaper[WS_MAX_PATH];
    COLORREF wallpaperColor;
    WsWallpaperFit fit;
    bool     lockPositions;
    WCHAR    fontFace[LF_FACESIZE];  /* independent controls */
    int      fontSize;
    int      uiScalePct;
    WsShortcutList desktopIcons;
} WsDesktopCfg;

/* ---------- App management ---------- */

typedef struct {
    WsShortcutList startApps;        /* start panel app list       */
    WsShortcutList pinnedTiles;
    WsShortcutList taskbarPinned;    /* taskbar shortcuts          */
    WCHAR          trayApps[WS_MAX_TRAY_APPS][WS_MAX_PATH];
    int            trayAppCount;
} WsAppMgmtCfg;

/* ---------- Master config ---------- */

typedef struct {
    DWORD      magic;                /* WS_CFG_MAGIC for validity check */
    DWORD      version;
    WsTaskbarCfg    taskbar;
    WsRevealBarCfg  revealBar;
    WsStartPanelCfg startPanel;
    WsCursorCfg     cursor;
    WsKeyboardCfg   keyboard;
    WsSkinningCfg   skinning;
    WsVisualCfg     visual;
    WsDesktopCfg    desktop;
    WsAppMgmtCfg    apps;
    WCHAR      configDir[WS_MAX_PATH];   /* resolved at runtime */
} AppConfig;

#define WS_CFG_MAGIC   0x57534C4Cu   /* "WSL" */
#define WS_CFG_VERSION 1

extern AppConfig g_cfg;

/* config_io.c */
bool Config_Load(void);
bool Config_Save(void);
bool Config_ExportJSON(const WCHAR *path);
bool Config_ImportJSON(const WCHAR *path);
void Config_FactoryReset(void);
void Config_ApplyDefaults(void);

#endif /* APP_CONFIG_H */
