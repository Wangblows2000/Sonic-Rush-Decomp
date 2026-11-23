/* Sonic Rush DC — Disc Maker (Windows 10 and later).
 *
 * A plain Win32 window with the standard Windows controls: pick (or drop) the Sonic Rush (Europe) ROM,
 * press one button, get SonicRushDC.cdi. The work is done by app\winbuild.py with the bundled Python
 * (app\python\python.exe); this program only starts it, shows its progress and reports the result.
 *
 * Build (MinGW-w64, see tools/dreamcast/winkit/make_winkit.sh):
 *   x86_64-w64-mingw32-windres srdcgui.rc -O coff -o srdcgui_res.o
 *   x86_64-w64-mingw32-gcc -O2 -municode -mwindows -o SonicRushDC.exe srdcgui.c srdcgui_res.o \
 *       -lcomctl32 -lcomdlg32 -lshell32 -lshlwapi
 */
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define APP_TITLE L"Sonic Rush DC \x2013 Disc Maker"
#define WM_APP_LINE (WM_APP + 1)   /* lParam: wchar_t* line (heap), from the reader thread */
#define WM_APP_EXIT (WM_APP + 2)   /* wParam: exit code */

enum { ID_BROWSE = 100, ID_MAKE, ID_DETAILS, ID_OPEN, ID_HELP, ID_ROMPATH, ID_LOG, ID_PROGRESS };

static HINSTANCE g_inst;
static HWND g_wnd, g_title, g_step1, g_rom, g_browse, g_step2, g_make, g_progress, g_status, g_sub, g_details,
    g_log, g_open, g_help;
static HFONT g_font, g_font_big, g_font_button, g_font_mono;
static int g_dpi = 96;
static wchar_t g_dir[MAX_PATH];        /* folder of this .exe */
static wchar_t g_rompath[MAX_PATH];
static wchar_t g_cdi[MAX_PATH];
static HANDLE g_proc, g_job;
static int g_running, g_show_log, g_error_shown;
static wchar_t g_error[1024];

static int S(int v) { return MulDiv(v, g_dpi, 96); }

static void set_status(const wchar_t *s) { SetWindowTextW(g_status, s); }

static void log_append(const wchar_t *s) {
    int n = GetWindowTextLengthW(g_log);
    if (n > 3000000) {   /* keep the log bounded: drop the first half */
        SendMessageW(g_log, EM_SETSEL, 0, n / 2);
        SendMessageW(g_log, EM_REPLACESEL, FALSE, (LPARAM)L"...\r\n");
        n = GetWindowTextLengthW(g_log);
    }
    SendMessageW(g_log, EM_SETSEL, n, n);
    SendMessageW(g_log, EM_REPLACESEL, FALSE, (LPARAM)s);
    SendMessageW(g_log, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
}

static void layout(void) {
    RECT rc;
    int w, x = S(20), y = S(16), cw;
    GetClientRect(g_wnd, &rc);
    w = rc.right - rc.left;
    cw = w - 2 * x;
    MoveWindow(g_title, x, y, cw, S(32), TRUE);
    y += S(44);
    MoveWindow(g_step1, x, y, cw, S(40), TRUE);
    y += S(44);
    MoveWindow(g_rom, x, y, cw - S(132), S(26), TRUE);
    MoveWindow(g_browse, x + cw - S(122), y - S(1), S(122), S(28), TRUE);
    y += S(42);
    MoveWindow(g_step2, x, y, cw, S(22), TRUE);
    y += S(28);
    MoveWindow(g_make, x, y, cw, S(52), TRUE);
    y += S(66);
    MoveWindow(g_progress, x, y, cw, S(22), TRUE);
    y += S(30);
    MoveWindow(g_status, x, y, cw, S(22), TRUE);
    y += S(22);
    MoveWindow(g_sub, x, y, cw, S(20), TRUE);
    y += S(30);
    MoveWindow(g_open, x, y, S(190), S(30), TRUE);
    MoveWindow(g_help, x + S(200), y, S(190), S(30), TRUE);
    MoveWindow(g_details, x + cw - S(130), y + S(4), S(130), S(22), TRUE);
    y += S(42);
    MoveWindow(g_log, x, y, cw, rc.bottom - y - S(16) > S(60) ? rc.bottom - y - S(16) : S(60), TRUE);
}

static void update_buttons(void) {
    EnableWindow(g_make, !g_running && g_rompath[0]);
    EnableWindow(g_browse, !g_running);
    DragAcceptFiles(g_wnd, !g_running);
    ShowWindow(g_open, g_cdi[0] && !g_running ? SW_SHOW : SW_HIDE);
    ShowWindow(g_help, g_cdi[0] && !g_running ? SW_SHOW : SW_HIDE);
}

static void resize_for_log(void) {
    RECT r;
    GetWindowRect(g_wnd, &r);
    int want = g_show_log ? S(720) : S(470);
    SetWindowPos(g_wnd, NULL, 0, 0, r.right - r.left, want, SWP_NOMOVE | SWP_NOZORDER);
    ShowWindow(g_log, g_show_log ? SW_SHOW : SW_HIDE);
}

static void set_rom(const wchar_t *p) {
    if (g_running)
        return;
    lstrcpynW(g_rompath, p, MAX_PATH);
    SetWindowTextW(g_rom, g_rompath);
    g_cdi[0] = 0;
    set_status(L"Ready. Press the big button!");
    SetWindowTextW(g_sub, L"");
    SendMessageW(g_progress, PBM_SETPOS, 0, 0);
    update_buttons();
}

static void browse(void) {
    wchar_t file[MAX_PATH] = L"";
    OPENFILENAMEW of;
    ZeroMemory(&of, sizeof(of));
    of.lStructSize = sizeof(of);
    of.hwndOwner = g_wnd;
    of.lpstrFilter = L"Nintendo DS ROM (*.nds)\0*.nds\0All files (*.*)\0*.*\0";
    of.lpstrFile = file;
    of.nMaxFile = MAX_PATH;
    of.lpstrTitle = L"Choose your Sonic Rush (Europe) ROM";
    of.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
    if (GetOpenFileNameW(&of))
        set_rom(file);
}

/* a single .nds next to the program is picked automatically */
static void autodetect_rom(void) {
    wchar_t pat[MAX_PATH], found[MAX_PATH] = L"";
    WIN32_FIND_DATAW fd;
    int n = 0;
    HANDLE h;
    swprintf(pat, MAX_PATH, L"%ls\\*.nds", g_dir);
    h = FindFirstFileW(pat, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            swprintf(found, MAX_PATH, L"%ls\\%ls", g_dir, fd.cFileName);
            n++;
        }
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    if (n == 1)
        set_rom(found);
}

typedef struct {
    HANDLE pipe;
} Reader;

static DWORD WINAPI reader_thread(LPVOID arg) {
    Reader *r = (Reader *)arg;
    char buf[4096], line[8192];
    int len = 0;
    DWORD got, code = 1;
    while (ReadFile(r->pipe, buf, sizeof(buf), &got, NULL) && got) {
        for (DWORD i = 0; i < got; i++) {
            char c = buf[i];
            if (c == '\n' || len == (int)sizeof(line) - 1) {
                int wn;
                wchar_t *w;
                if (len && line[len - 1] == '\r')
                    len--;
                line[len] = 0;
                wn = MultiByteToWideChar(CP_UTF8, 0, line, -1, NULL, 0);
                w = (wchar_t *)malloc(sizeof(wchar_t) * (wn ? wn : 1));
                if (w) {
                    if (!wn || !MultiByteToWideChar(CP_UTF8, 0, line, -1, w, wn))
                        w[0] = 0;
                    PostMessageW(g_wnd, WM_APP_LINE, 0, (LPARAM)w);
                }
                len = 0;
            } else
                line[len++] = c;
        }
    }
    CloseHandle(r->pipe);
    free(r);
    WaitForSingleObject(g_proc, INFINITE);
    GetExitCodeProcess(g_proc, &code);
    PostMessageW(g_wnd, WM_APP_EXIT, code, 0);
    return 0;
}

static int file_exists(const wchar_t *p) {
    DWORD a = GetFileAttributesW(p);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static void start_build(void) {
    wchar_t python[MAX_PATH], script[MAX_PATH], out[MAX_PATH], app[MAX_PATH], *cmd;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    HANDLE rd, wr;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jl;
    Reader *r;
    swprintf(app, MAX_PATH, L"%ls\\app", g_dir);
    swprintf(python, MAX_PATH, L"%ls\\python\\python.exe", app);
    swprintf(script, MAX_PATH, L"%ls\\winbuild.py", app);
    swprintf(out, MAX_PATH, L"%ls\\Your Dreamcast disc", g_dir);
    if (!file_exists(python) || !file_exists(script)) {
        MessageBoxW(g_wnd,
                    L"Some files of the disc maker are missing.\n\n"
                    L"Unzip the whole SonicRushDC zip file first (right-click it, \"Extract All...\"), "
                    L"then start SonicRushDC.exe from the unzipped folder.",
                    APP_TITLE, MB_ICONERROR);
        return;
    }
    if (!file_exists(g_rompath)) {
        MessageBoxW(g_wnd, L"The ROM file is gone. Choose it again.", APP_TITLE, MB_ICONWARNING);
        return;
    }
    cmd = (wchar_t *)malloc(sizeof(wchar_t) * 4 * MAX_PATH + 256);
    swprintf(cmd, 4 * MAX_PATH + 128, L"\"%ls\" -u -X utf8 \"%ls\" --rom \"%ls\" --out \"%ls\"", python, script,
             g_rompath, out);
    if (!CreatePipe(&rd, &wr, &sa, 0)) {
        free(cmd);
        return;
    }
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = wr;
    si.hStdError = wr;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    /* all helper processes share one hidden console (no flashing windows) and one job (Stop kills them all) */
    if (!CreateProcessW(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW | CREATE_SUSPENDED, NULL, app, &si, &pi)) {
        wchar_t m[512];
        swprintf(m, 512, L"Could not start the disc maker (error %lu).", GetLastError());
        MessageBoxW(g_wnd, m, APP_TITLE, MB_ICONERROR);
        CloseHandle(rd);
        CloseHandle(wr);
        free(cmd);
        return;
    }
    free(cmd);
    CloseHandle(wr);
    g_job = CreateJobObjectW(NULL, NULL);
    if (g_job) {
        ZeroMemory(&jl, sizeof(jl));
        jl.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        SetInformationJobObject(g_job, JobObjectExtendedLimitInformation, &jl, sizeof(jl));
        AssignProcessToJobObject(g_job, pi.hProcess);
    }
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    g_proc = pi.hProcess;
    g_running = 1;
    g_cdi[0] = 0;
    g_error[0] = 0;
    g_error_shown = 0;
    SetWindowTextW(g_log, L"");
    SendMessageW(g_progress, PBM_SETPOS, 0, 0);
    set_status(L"Starting...");
    SetWindowTextW(g_sub, L"");
    SetWindowTextW(g_make, L"Working on it... (about 5 minutes)");
    update_buttons();
    r = (Reader *)malloc(sizeof(Reader));
    r->pipe = rd;
    CloseHandle(CreateThread(NULL, 0, reader_thread, r, 0, NULL));
}

/* "@@ERROR text" uses \n for line breaks */
static void unescape(wchar_t *s) {
    wchar_t *d = s;
    for (; *s; s++) {
        if (s[0] == L'\\' && s[1] == L'n') {
            *d++ = L'\n';
            s++;
        } else
            *d++ = *s;
    }
    *d = 0;
}

static void handle_line(wchar_t *s) {
    if (!wcsncmp(s, L"@@STEP ", 7)) {
        int n = 0, total = 0, off = 0;
        wchar_t msg[512];
        if (swscanf(s + 7, L"%d %d %n", &n, &total, &off) >= 2) {
            swprintf(msg, 512, L"Step %d of %d: %ls", n, total, s + 7 + off);
            set_status(msg);
            SetWindowTextW(g_sub, L"");
        }
    } else if (!wcsncmp(s, L"@@PCT ", 6)) {
        SendMessageW(g_progress, PBM_SETPOS, _wtoi(s + 6), 0);
    } else if (!wcsncmp(s, L"@@SUB ", 6)) {
        wchar_t msg[512];
        swprintf(msg, 512, L"Now %ls...", s + 6);
        SetWindowTextW(g_sub, msg);
    } else if (!wcsncmp(s, L"@@DONE ", 7)) {
        lstrcpynW(g_cdi, s + 7, MAX_PATH);
    } else if (!wcsncmp(s, L"@@ERROR ", 8)) {
        lstrcpynW(g_error, s + 8, 1024);
        unescape(g_error);
    }
    log_append(s);
}

static void finished(DWORD code) {
    CloseHandle(g_proc);
    g_proc = NULL;
    if (g_job) {
        CloseHandle(g_job);
        g_job = NULL;
    }
    g_running = 0;
    SetWindowTextW(g_make, L"Make my Dreamcast disc!");
    if (code == 0 && g_cdi[0]) {
        SendMessageW(g_progress, PBM_SETPOS, 100, 0);
        set_status(L"All done! Your Dreamcast disc is ready.");
        SetWindowTextW(g_sub, L"It is called SonicRushDC.cdi, in the folder \"Your Dreamcast disc\".");
        update_buttons();
        MessageBeep(MB_OK);
        MessageBoxW(g_wnd,
                    L"All done!\n\nYour disc is SonicRushDC.cdi in the folder \"Your Dreamcast disc\".\n\n"
                    L"Open it in the Flycast emulator, or burn it to a CD-R for a Dreamcast.\n"
                    L"Keep it for yourself: it contains the game.",
                    APP_TITLE, MB_ICONINFORMATION);
    } else {
        set_status(L"The disc could not be made.");
        SetWindowTextW(g_sub, L"Press \"Show details\" to see what happened.");
        update_buttons();
        if (!g_error_shown) {
            g_error_shown = 1;
            MessageBoxW(g_wnd, g_error[0] ? g_error : L"Something went wrong while making the disc.", APP_TITLE,
                        MB_ICONERROR);
        }
    }
}

static HWND make_ctl(const wchar_t *cls, const wchar_t *text, DWORD style, int id, HFONT f) {
    HWND h = CreateWindowExW(0, cls, text, WS_CHILD | WS_VISIBLE | style, 0, 0, 10, 10, g_wnd,
                             (HMENU)(INT_PTR)id, g_inst, NULL);
    SendMessageW(h, WM_SETFONT, (WPARAM)f, TRUE);
    return h;
}

static HFONT ui_font(int pt, int weight, const wchar_t *face) {
    return CreateFontW(-MulDiv(pt, g_dpi, 72), 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, face);
}

static void open_folder(void) {
    wchar_t args[MAX_PATH + 32];
    swprintf(args, MAX_PATH + 32, L"/select,\"%ls\"", g_cdi);
    ShellExecuteW(g_wnd, L"open", L"explorer.exe", args, NULL, SW_SHOWNORMAL);
}

static void open_help(void) {
    wchar_t p[MAX_PATH];
    swprintf(p, MAX_PATH, L"%ls\\README.txt", g_dir);
    ShellExecuteW(g_wnd, L"open", p, NULL, NULL, SW_SHOWNORMAL);
}

static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM wp, LPARAM lp) {
    switch (m) {
    case WM_CREATE:
        g_wnd = h;
        g_font = ui_font(10, FW_NORMAL, L"Segoe UI");
        g_font_big = ui_font(16, FW_SEMIBOLD, L"Segoe UI");
        g_font_button = ui_font(13, FW_SEMIBOLD, L"Segoe UI");
        g_font_mono = ui_font(9, FW_NORMAL, L"Consolas");
        g_title = make_ctl(L"STATIC", L"Make your Sonic Rush Dreamcast disc", SS_LEFT, 0, g_font_big);
        g_step1 = make_ctl(L"STATIC",
                           L"1.  Choose your Sonic Rush (Europe) ROM file (.nds). "
                           L"You can also drag the file onto this window.",
                           SS_LEFT, 0, g_font);
        g_rom = make_ctl(L"EDIT", L"", WS_BORDER | ES_READONLY | ES_AUTOHSCROLL, ID_ROMPATH, g_font);
        SendMessageW(g_rom, EM_SETCUEBANNER, TRUE, (LPARAM)L"No ROM chosen yet");
        g_browse = make_ctl(L"BUTTON", L"Choose ROM...", BS_PUSHBUTTON | WS_TABSTOP, ID_BROWSE, g_font);
        g_step2 = make_ctl(L"STATIC", L"2.  Press the big button and wait. It takes about 5 minutes.", SS_LEFT,
                           0, g_font);
        g_make = make_ctl(L"BUTTON", L"Make my Dreamcast disc!", BS_DEFPUSHBUTTON | WS_TABSTOP, ID_MAKE,
                          g_font_button);
        g_progress = make_ctl(PROGRESS_CLASSW, L"", PBS_SMOOTH, ID_PROGRESS, g_font);
        SendMessageW(g_progress, PBM_SETRANGE32, 0, 100);
        g_status = make_ctl(L"STATIC", L"Choose your ROM to begin.", SS_LEFT | SS_ENDELLIPSIS, 0, g_font);
        g_sub = make_ctl(L"STATIC", L"", SS_LEFT | SS_ENDELLIPSIS, 0, g_font);
        g_open = make_ctl(L"BUTTON", L"Show my disc", BS_PUSHBUTTON | WS_TABSTOP, ID_OPEN, g_font);
        g_help = make_ctl(L"BUTTON", L"How do I play it?", BS_PUSHBUTTON | WS_TABSTOP, ID_HELP, g_font);
        g_details = make_ctl(L"BUTTON", L"Show details", BS_AUTOCHECKBOX | WS_TABSTOP, ID_DETAILS, g_font);
        g_log = make_ctl(L"EDIT", L"",
                         WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL |
                             ES_AUTOHSCROLL,
                         ID_LOG, g_font_mono);
        SendMessageW(g_log, EM_SETLIMITTEXT, 8000000, 0);
        ShowWindow(g_log, SW_HIDE);
        layout();
        update_buttons();
        autodetect_rom();
        return 0;
    case WM_SIZE:
        layout();
        return 0;
    case WM_GETMINMAXINFO: {
        MINMAXINFO *mm = (MINMAXINFO *)lp;
        mm->ptMinTrackSize.x = S(560);
        mm->ptMinTrackSize.y = S(470);
        return 0;
    }
    case WM_CTLCOLORSTATIC:
        if ((HWND)lp == g_rom)
            break;
        SetBkMode((HDC)wp, TRANSPARENT);
        if ((HWND)lp == g_title)
            SetTextColor((HDC)wp, RGB(0x10, 0x3c, 0x9c));
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BROWSE:
            browse();
            return 0;
        case ID_MAKE:
            if (!g_running && g_rompath[0])
                start_build();
            return 0;
        case ID_OPEN:
            open_folder();
            return 0;
        case ID_HELP:
            open_help();
            return 0;
        case ID_DETAILS:
            g_show_log = SendMessageW(g_details, BM_GETCHECK, 0, 0) == BST_CHECKED;
            resize_for_log();
            return 0;
        }
        break;
    case WM_DROPFILES: {
        wchar_t p[MAX_PATH];
        if (DragQueryFileW((HDROP)wp, 0, p, MAX_PATH))
            set_rom(p);
        DragFinish((HDROP)wp);
        return 0;
    }
    case WM_APP_LINE:
        handle_line((wchar_t *)lp);
        free((void *)lp);
        return 0;
    case WM_APP_EXIT:
        finished((DWORD)wp);
        return 0;
    case WM_CLOSE:
        if (g_running) {
            if (MessageBoxW(h, L"The disc is not finished yet. Stop and close?", APP_TITLE,
                            MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
                return 0;
            if (g_job)
                TerminateJobObject(g_job, 1);
            else if (g_proc)
                TerminateProcess(g_proc, 1);
        }
        DestroyWindow(h);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(h, m, wp, lp);
}

int WINAPI wWinMain(HINSTANCE inst, HINSTANCE prev, PWSTR cmdline, int show) {
    WNDCLASSEXW wc;
    INITCOMMONCONTROLSEX icc = {sizeof(icc), ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES};
    MSG msg;
    HDC dc;
    (void)prev;
    (void)cmdline;
    g_inst = inst;
    InitCommonControlsEx(&icc);
    GetModuleFileNameW(NULL, g_dir, MAX_PATH);
    PathRemoveFileSpecW(g_dir);
    dc = GetDC(NULL);
    g_dpi = GetDeviceCaps(dc, LOGPIXELSY);
    ReleaseDC(NULL, dc);
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = wndproc;
    wc.hInstance = inst;
    wc.hIcon = LoadIconW(inst, MAKEINTRESOURCEW(1));
    wc.hIconSm = (HICON)LoadImageW(inst, MAKEINTRESOURCEW(1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
                                   GetSystemMetrics(SM_CYSMICON), 0);
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SonicRushDCDiscMaker";
    RegisterClassExW(&wc);
    CreateWindowExW(WS_EX_ACCEPTFILES, wc.lpszClassName, APP_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    CW_USEDEFAULT, S(640), S(470), NULL, NULL, inst, NULL);
    ShowWindow(g_wnd, show);
    UpdateWindow(g_wnd);
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        if (!IsDialogMessageW(g_wnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return (int)msg.wParam;
}
