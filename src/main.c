#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <stdio.h>

#include "common.h"
#include "app_config.h"
#include "wine_bridge.h"
#include "paths.h"
#include "single_instance.h"
#include "gdiplus_ctx.h"
#include "anim_engine.h"
#include "theme_manager.h"
#include "taskbar.h"
#include "start_panel.h"
#include "desktop_win.h"
#include "reveal_bar.h"
#include "skin_hook.h"
#include "cursor_engine.h"
#include "kb_autotrigger.h"
#include "settings_app.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define WM_WSL_SETTINGS   (WM_APP + 1)
#define WM_WSL_THEMECHG   (WM_APP + 2)

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrev,
                      LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrev);

    /* --- 1. Single instance ---------------------------------------- */
    HWND existing = SingleInstance_ActivateOrExit(L"WinlatorShell_Mutex");
    if (existing) {
        /* Second launch => open Settings GUI instead of duplicating shell */
        SendMessage(existing, WM_WSL_SETTINGS, 0, 0);
        return 0;
    }

    /* --- 2. Resolve Wine/Winlator environment ---------------------- */
    Paths_ResolveWinePrefix();          /* finds drive_c, detects Winlator */
    Config_ApplyDefaults();
    if (!Config_Load()) {
        LOG_WARN(L"No config found, using defaults");
    }

    /* --- 3. Common controls v6 + GDI+ ------------------------------ */
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_STANDARD_CLASSES |
                                 ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES };
    InitCommonControlsEx(&icc);
    GdiPlusCtx_Init();                  /* GDI+ startup, shared caches */
    AnimEngine_Init(60);                /* 60 Hz tween clock */

    /* --- 4. Theme engine before any surface exists ----------------- */
    ThemeManager_Load(g_cfg.visual.theme, g_cfg.visual.mode);

    /* --- 5. Hide Winlator/Wine explorer.exe taskbar ---------------- */
    WineBridge_HideNativeTaskbar();     /* FindWindow(L"Shell_TrayWnd") -> SW_HIDE;
                                           also hides "TrayNotifyWnd"/"Start" */

    /* --- 6. Create surfaces (z-order: desktop < reveal < taskbar) -- */
    DesktopWin_Create(hInst);           /* no-op unless Full Desktop Mode on */
    Taskbar_Create(hInst);              /* custom taskbar, 4-edge capable */
    StartPanel_Create(hInst);           /* panel hidden until Start clicked */
    RevealBar_Create(hInst);            /* edge trigger, hidden unless enabled */

    /* --- 7. Optional engines --------------------------------------- */
    if (g_cfg.skinning.enabled)     SkinHook_Install();
    if (g_cfg.cursor.enabled)       CursorEngine_Start();
    KeyboardAutoTrigger_Start();        /* cheap polling thread, always on;
                                           individual methods gated by cfg */

    /* --- 8. Register for config-change broadcasts ------------------- */
    /* Settings GUI sends WM_WSL_THEMECHG; modules re-read g_cfg and repaint */

    /* --- 9. Message loop -------------------------------------------- */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        /* modeless settings window + keyboard accelerators */
        if (SettingsApp_IsDialogMessage(&msg)) continue;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* --- 10. Clean shutdown ----------------------------------------- */
    KeyboardAutoTrigger_Stop();
    CursorEngine_Stop();
    SkinHook_Remove();
    RevealBar_Destroy();
    StartPanel_Destroy();
    Taskbar_Destroy();
    DesktopWin_Destroy();
    AnimEngine_Shutdown();
    GdiPlusCtx_Shutdown();
    Config_Save();
    return (int)msg.wParam;
}
