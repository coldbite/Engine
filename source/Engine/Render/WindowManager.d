/*
 * coldbite Engine
 *
 * Copyright (c) 2024. All rights reserved.
 *
 * This source code is licensed under the MIT License found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @author  Adrian Preuß
 * @version 1.0.0
 * @created 20.11.2024
 * @updated N/A
 */

module Engine.Render.WindowManager;

import core.sys.windows.windows;
import core.sys.windows.winuser;
import core.runtime;
import core.stdc.stdio;
import core.stdc.string;
import std.stdio;
import std.algorithm;
import std.string;
import std.utf;
import core.thread;

pragma(lib, "user32.lib");
extern(Windows) nothrow HBRUSH CreateSolidBrush(uint color);
extern(Windows) nothrow LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /*
        WM_COMMAND, WM_PAINT
    */

    /* Remove the Cursor */
    if(msg == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT) {
        SetCursor(NULL);
        return TRUE;

    /* Disable Background-Painter */
    } else if(msg == WM_ERASEBKGND) {
        return TRUE;
    } else if(msg == WM_PAINT) {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return FALSE;

    /* Exit */
    } else if(msg == WM_DESTROY) {
        PostQuitMessage(FALSE);
        return FALSE;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

class WindowManager : Thread {
    private bool running = false;
    private HWND hwnd;
    private WNDCLASSA wc = void;
    private RECT originalRect;
    private ULONG originalStyle;
    private ULONG style = WS_OVERLAPPEDWINDOW;
    private void delegate() @system onShow = null;
    private string title = "MyGame";
    private int width = 800;
    private int height = 600;
    private int x = 0;
    private int y = 0;

    this() {
        super(&run);
        start();
    }

    ~this() {
        writeln("Shutdown WindowManager");
        join();
    }

    public bool isActive() {
        return running;
    }

    public void show(void delegate() @system callback) {
        onShow = callback;
    }

    public void setTitle(string value) {
        title = value;
    }

    public void setSize(int w, int h) {
        width   = w;
        height  = h;

        SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    public void setPosition(int new_x, int new_y) {
        x = (new_x == -1 ? CW_USEDEFAULT : new_x);
        y = (new_y == -1 ? CW_USEDEFAULT : new_y);

        SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
    }

    public void setFullscreen(bool state) {
        if(state) {
            GetWindowRect(hwnd, &originalRect);

            originalStyle       = GetWindowLongA(hwnd, GWL_STYLE);
            style               = WS_POPUP;
            x                   = 0;
            y                   = 0;
            width               = GetSystemMetrics(SM_CXSCREEN); // Breite des Bildschirms
            height              = GetSystemMetrics(SM_CYSCREEN); // Höhe des Bildschirms

            SetWindowLongA(hwnd, GWL_STYLE, style);
            SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
        } else {
            style = originalStyle;
            SetWindowLongA(hwnd, GWL_STYLE, style);

            x                   = originalRect.left;
            y                   = originalRect.top;
            width               = originalRect.right - originalRect.left;
            height              = originalRect.bottom - originalRect.top;

            SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
        }
    }

    public HWND getContext() {
        return hwnd;
    }

    private void run() {
        HINSTANCE hInstance = GetModuleHandleA(null);
        wc.lpfnWndProc      = &WindowProc;
        wc.style            = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        wc.hInstance        = hInstance;
        wc.lpszClassName    = "Window".ptr;
        wc.hIcon            = LoadIcon(cast(HINSTANCE) null, IDI_APPLICATION);
        wc.hbrBackground    = null;

        if (!RegisterClassA(&wc)) {
            // @ToDo Error?
            writeln("Failed to register window class. Error code: ", GetLastError());
            return;
        }

        hwnd = CreateWindowExA(0, "Window".ptr, title.ptr, style, x, y, width, height, null, null, hInstance, null);

        if (hwnd is null) {
            MessageBoxA(null, "Failed to create window", "Error", MB_ICONERROR);
            return;
        }

        UpdateWindow(hwnd);

        MSG msg;
        running = true;
        while(running) {
            if(onShow !is null) {
                ShowWindow(hwnd, SW_SHOW);
                onShow();
                onShow = null;
                continue;
            }

            int result = GetMessageA(&msg, null, 0, 0);

            TranslateMessage(&msg);
            DispatchMessageA(&msg);

            /* WM_PAINT */

            if(msg.message == WM_QUIT || msg.message == WM_DESTROY) {
                running = false;
                break;
            } else {
                //writeln("Message received: ", msg.message, " wParam: ", msg.wParam, " lParam: ", msg.lParam);
            }
        }

        // Cleanup? Or make these in destruct?
    }
}