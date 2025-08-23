#include "NativeWindow.h"
#include "../Graphics/OpenGL/OpenGL.h"
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
    #include <windows.h>
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
        , wglSwapIntervalEXT(nullptr)
        , vsyncSupported(false)
        , currentVSyncState(false)
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
#endif
    }

    void NativeWindow::SetIcon(const std::string& iconPath) {
        properties.icon = iconPath;

        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        // Big Icon (Taskbar / Alt+Tab)
        HICON hIconBig = (HICON) LoadImage(
            nullptr,
            iconPath.c_str(),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON),
            LR_LOADFROMFILE
        );

        // Small Icon (Title)
        HICON hIconSmall = (HICON) LoadImage(
            nullptr,
            iconPath.c_str(),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON),
            LR_LOADFROMFILE
        );

        if(hIconBig) {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIconBig);
        }

        if(hIconSmall) {
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);
        }
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
#else
        // Suppress unused parameter warnings on non-Windows platforms
        (void)x;
        (void)y;
#endif
    }

    void NativeWindow::SetFullscreen(bool fullscreen) {
        properties.fullscreen = fullscreen;
        // TODO: Implement fullscreen toggle
    }

    void NativeWindow::SetVSync(bool enabled) {
        properties.vsync = enabled;

#ifdef _WIN32
        if(!hglrc) {
            std::cout << "[NativeWindow] Warning: No OpenGL context for VSync" << std::endl;
            return;
        }

        if(!vsyncSupported) {
            std::cout << "[NativeWindow] Warning: VSync not supported" << std::endl;
            return;
        }

        if(currentVSyncState == enabled) {
            return; // Already in desired state
        }

        if(wglSwapIntervalEXT) {
            BOOL result = wglSwapIntervalEXT(enabled ? 1 : 0);
            if(result) {
                currentVSyncState = enabled;
                std::cout << "[NativeWindow] VSync: " << (enabled ? "ON" : "OFF") << std::endl;
            } else {
                std::cout << "[NativeWindow] Error: Failed to set VSync" << std::endl;
            }
        }
#else
        std::cout << "[NativeWindow] VSync: " << (enabled ? "ON" : "OFF") << " (Platform not supported)" << std::endl;
#endif
    }

    bool NativeWindow::SetupRenderingContext(const std::string& api) {
        if(!isCreated) {
            return false;
        }

        std::cout << "[NativeWindow] Setting up rendering context for: " << api << std::endl;

        if(api == "OpenGL") {
#ifdef _WIN32
            // Basic OpenGL context setup (low-level)
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

            // Make context current to initialize VSync
            if(!wglMakeCurrent(hdc, hglrc)) {
                std::cout << "[NativeWindow] Warning: Could not make context current for VSync init" << std::endl;
            }

            // Initialize VSync extension
            wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(
                reinterpret_cast<void*>(wglGetProcAddress("wglSwapIntervalEXT"))
            );

            vsyncSupported = (wglSwapIntervalEXT != nullptr);

            if(vsyncSupported) {
                std::cout << "[NativeWindow] VSync extension available" << std::endl;
                // Apply VSync setting from properties
                SetVSync(properties.vsync);
            } else {
                std::cout << "[NativeWindow] Warning: VSync extension not available" << std::endl;
            }

            // Now initialize our OpenGL wrapper
            // NOTE: We need to get a shared_ptr to 'this', but we can't use shared_from_this here
            // For now, we'll pass the current instance directly - this needs to be refactored
            auto OGL    = std::make_shared<Graphics::OpenGL::OpenGL>();

            // HACK: Create a shared_ptr that doesn't delete the object when it goes out of scope
            // This is not ideal, but works for now until we refactor to proper shared_ptr management
            std::shared_ptr<NativeWindow> self(this, [](NativeWindow*){});
            return OGL->Init(self);
#else
            std::cout << "[NativeWindow] OpenGL context creation not supported on this platform" << std::endl;
            return false;
#endif
        }

        std::cout << "[NativeWindow] Rendering API '" << api << "' not implemented yet" << std::endl;
        return false;
    }

    void NativeWindow::MakeContextCurrent() {
        if(!isCreated) {
            return;
        }

#ifdef _WIN32
        if(hglrc && hdc) {
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

        if(properties.icon.empty()) {
            windowClass.hIcon           = LoadIcon(nullptr, IDI_APPLICATION);
        } else {
            windowClass.hIcon = (HICON) LoadImage(
                nullptr,
                properties.icon.c_str(),
                IMAGE_ICON,
                0, 0,
                LR_LOADFROMFILE | LR_DEFAULTSIZE
            );
        }

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
            case WM_SIZE: {
                int newWidth = LOWORD(lParam);
                int newHeight = HIWORD(lParam);

                properties.width = newWidth;
                properties.height = newHeight;

                if(resizeCallback) {
                    resizeCallback(properties.width, properties.height);
                }
            } break;
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
            
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
                if(mouseButtonCallback) {
                    float x = static_cast<float>(LOWORD(lParam));
                    float y = static_cast<float>(HIWORD(lParam));
                    int action = (uMsg == WM_LBUTTONDOWN) ? WM_LBUTTONDOWN : WM_LBUTTONUP;
                    mouseButtonCallback(0, action, x, y); // 0 = left button
                }
            break;
            
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                if(mouseButtonCallback) {
                    float x = static_cast<float>(LOWORD(lParam));
                    float y = static_cast<float>(HIWORD(lParam));
                    int action = (uMsg == WM_RBUTTONDOWN) ? WM_RBUTTONDOWN : WM_RBUTTONUP;
                    mouseButtonCallback(1, action, x, y); // 1 = right button
                }
            break;
            
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
                if(mouseButtonCallback) {
                    float x = static_cast<float>(LOWORD(lParam));
                    float y = static_cast<float>(HIWORD(lParam));
                    int action = (uMsg == WM_MBUTTONDOWN) ? WM_MBUTTONDOWN : WM_MBUTTONUP;
                    mouseButtonCallback(2, action, x, y); // 2 = middle button
                }
            break;
            
            case WM_MOUSEMOVE:
                if(mouseMoveCallback) {
                    float x = static_cast<float>(LOWORD(lParam));
                    float y = static_cast<float>(HIWORD(lParam));
                    mouseMoveCallback(x, y);
                }
            break;
            
            case WM_MOUSEWHEEL:
                if(mouseScrollCallback) {
                    float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
                    mouseScrollCallback(0.0f, delta); // Vertical scroll
                }
            break;
            
            case WM_MOUSEHWHEEL:
                if(mouseScrollCallback) {
                    float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
                    mouseScrollCallback(delta, 0.0f); // Horizontal scroll
                }
            break;
        }
    }
#endif
}

namespace Engine {
    void NativeWindow::UpdateWindowProperties(int width, int height) {
        properties.width = width;
        properties.height = height;
    }
}
