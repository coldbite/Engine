#include "NativeWindow.h"
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
    #include <wingdi.h>
    #include <gl/gl.h>
#endif

namespace Engine {

#ifdef _WIN32
    static std::unordered_map<HWND, NativeWindow*> windowMap;
#endif

    NativeWindow::NativeWindow()
        : isCreated(false)
#ifdef _WIN32
        , hwnd(nullptr)
        , hdc(nullptr)
        , hglrc(nullptr)
#endif
    {
        /* Do Nothing */
    }

    NativeWindow::~NativeWindow() {
        Destroy();
    }

    bool NativeWindow::Create(const WindowProperties& props) {
        if(isCreated) {
            return true;
        }

        properties = props;

        if(!CreatePlatformWindow()) {
            std::cout << "[NativeWindow] Failed to create platform window" << std::endl;
            return false;
        }

        isCreated = true;

        return true;
    }

    void NativeWindow::Destroy() {
        if(!isCreated) {
            return;
        }

        DestroyPlatformWindow();
        isCreated = false;
    }

    bool NativeWindow::IsValid() const {
        return isCreated;
    }

    void NativeWindow::Show() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
#endif
    }

    void NativeWindow::Hide() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        ShowWindow(hwnd, SW_HIDE);
#endif
    }

    void NativeWindow::SwapBuffers() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        if(hdc) {
            ::SwapBuffers(hdc);
        }
#endif
    }

    void NativeWindow::PollEvents() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        MSG msg;

        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
#elif defined(__CYGWIN__)
        // Mock event polling for Cygwin
        // No actual events to poll in mock mode
#endif
    }

    void NativeWindow::SetTitle(const std::string& title) {
        properties.title = title;

        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        SetWindowTextA(hwnd, title.c_str());
#endif
    }

    void NativeWindow::SetSize(int width, int height) {
        properties.width    = width;
        properties.height   = height;

        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
#endif
    }

    void NativeWindow::SetPosition(int x, int y) {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
#endif
    }

    void NativeWindow::SetFullscreen(bool fullscreen) {
        properties.fullscreen = fullscreen;
        // TODO: Implement fullscreen toggle
    }

    void NativeWindow::SetVSync(bool enabled) {
        properties.vsync = enabled;
        // TODO: Implement VSync control through OpenGL/DirectX
        std::cout << "[NativeWindow] VSync: " << (enabled ? "ON" : "OFF") << std::endl;
    }

    bool NativeWindow::SetupRenderingContext(const std::string& api) {
        if(!isCreated) {
            return false;
        }

#ifdef _WIN32
        if(api == "OpenGL") {
            // @ToDo Init auslagern!
            //    /Engine/Graphics/OpenGL/OpenGL.cpp
            // Und hier einfach
            //      OpenGL::Init(nativeWindow);
            // nutzen!

            // Basic OpenGL context setup
            PIXELFORMATDESCRIPTOR pfd = {};
            pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion        = 1;
            pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType      = PFD_TYPE_RGBA;
            pfd.cColorBits      = 32;
            pfd.cDepthBits      = 24;
            pfd.cStencilBits    = 8;

            int pixelFormat = ChoosePixelFormat(hdc, &pfd);

            if(!SetPixelFormat(hdc, pixelFormat, &pfd)) {
                std::cout << "[NativeWindow] Failed to set pixel format" << std::endl;
                return false;
            }

            hglrc = wglCreateContext(hdc);

            if(!hglrc) {
                std::cout << "[NativeWindow] Failed to create OpenGL context" << std::endl;
                return false;
            }

            std::cout << "[NativeWindow] OpenGL context created successfully" << std::endl;
            return true;
        }
#elif defined(__CYGWIN__)
        // Mock rendering context for Cygwin
        std::cout << "[NativeWindow] Mock rendering context setup for: " << api << std::endl;
        return true;
#endif

        std::cout << "[NativeWindow] Rendering API '" << api << "' not implemented yet" << std::endl;
        return false;
    }

    void NativeWindow::MakeContextCurrent() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        // @ToDO api specific???
        if(hglrc) {
            wglMakeCurrent(hdc, hglrc);
        }
#endif
    }

    bool NativeWindow::CreatePlatformWindow() {
#ifdef _WIN32
        // Register window class
        const char* className       = "EngineWindowClass";

        windowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.lpfnWndProc     = WindowProc;
        windowClass.cbClsExtra      = 0;
        windowClass.cbWndExtra      = 0;
        windowClass.hInstance       = GetModuleHandle(nullptr);
        windowClass.hIcon           = LoadIcon(nullptr, IDI_APPLICATION);
        windowClass.hCursor         = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
        windowClass.lpszMenuName    = nullptr;
        windowClass.lpszClassName   = className;

        if(!RegisterClass(&windowClass)) {
            DWORD error = GetLastError();

            if(error != ERROR_CLASS_ALREADY_EXISTS) {
                std::cout << "[NativeWindow] Failed to register window class, error: " << error << std::endl;
                return false;
            }
        }

        // Calculate window size including borders
        RECT rect   = { 0, 0, properties.width, properties.height };
        DWORD style = WS_OVERLAPPEDWINDOW;

        if(!properties.resizable) {
            style   = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        }

        AdjustWindowRect(&rect, style, FALSE);

        // Create window
        hwnd = CreateWindowA(
            className,
            properties.title.c_str(),
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            nullptr
        );

        if(!hwnd) {
            std::cout << "[NativeWindow] Failed to create window" << std::endl;
            return false;
        }

        // Get device context
        hdc = GetDC(hwnd);

        if(!hdc) {
            std::cout << "[NativeWindow] Failed to get device context" << std::endl;
            return false;
        }

        // Store window instance for message handling
        windowMap[hwnd] = this;

        return true;
#elif defined(__CYGWIN__)
        // Mock window creation for Cygwin environment
        std::cout << "[NativeWindow] Creating mock window in Cygwin environment" << std::endl;
        std::cout << "[NativeWindow] Mock window: " << properties.title
                  << " (" << properties.width << "x" << properties.height << ")" << std::endl;
        return true;
#else
        std::cout << "[NativeWindow] Platform not supported" << std::endl;
        return false;
#endif
    }

    void NativeWindow::DestroyPlatformWindow() {
#ifdef _WIN32
        if(hglrc) {
            // @ToDo api specific???
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
            hglrc = nullptr;
        }

        if(hdc) {
            ReleaseDC(hwnd, hdc);
            hdc = nullptr;
        }

        if(hwnd) {
            windowMap.erase(hwnd);
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }

        UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
#elif defined(__CYGWIN__)
        // Mock window cleanup for Cygwin
        std::cout << "[NativeWindow] Mock window cleanup in Cygwin environment" << std::endl;
#endif
    }

    void NativeWindow::UpdateWindowProperties() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        RECT rect;
        GetClientRect(hwnd, &rect);
        properties.width    = rect.right - rect.left;
        properties.height   = rect.bottom - rect.top;
#endif
    }

#ifdef _WIN32
    LRESULT CALLBACK NativeWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        auto it = windowMap.find(hwnd);

        if(it != windowMap.end()) {
            it->second->HandleWindowMessage(uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void NativeWindow::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch(uMsg) {
            case WM_SIZE:
                properties.width    = LOWORD(lParam);
                properties.height   = HIWORD(lParam);

                if(resizeCallback) {
                    resizeCallback(properties.width, properties.height);
                }
            break;
            case WM_CLOSE:
                if(closeCallback) {
                    closeCallback();
                }
            break;
            case WM_KEYDOWN:
            case WM_KEYUP:
                if(keyCallback) {
                    int action = (uMsg == WM_KEYDOWN) ? 1 : 0;
                    keyCallback(static_cast<int>(wParam), action);
                }
            break;
        }
    }
#endif
}
