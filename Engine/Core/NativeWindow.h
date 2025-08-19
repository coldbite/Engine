#pragma once

#include <string>
#include <functional>
#include <memory>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace Engine {
    struct WindowProperties {
        std::string title	= "Coldbite";
        int width		= 800;
        int height		= 600;
        bool resizable		= false;
        bool vsync		= false;
        bool fullscreen		= false;
    };

    class NativeWindow {
    public:
        NativeWindow();
        ~NativeWindow();

        // Window lifecycle
        bool Create(const WindowProperties& props);
        void Destroy();
        bool IsValid() const;

        // Window operations
        void Show();
        void Hide();
        void SwapBuffers();
        void PollEvents();

        // Window properties
        void SetTitle(const std::string& title);
        void SetSize(int width, int height);
        void SetPosition(int x, int y);
        void SetFullscreen(bool fullscreen);
        void SetVSync(bool enabled);

        // Getters
        int GetWidth() const { return properties.width; }
        int GetHeight() const { return properties.height; }
        const std::string& GetTitle() const { return properties.title; }
        bool IsFullscreen() const { return properties.fullscreen; }
        bool IsVSyncEnabled() const { return properties.vsync; }
        const WindowProperties& GetProperties() const { return properties; }

        // Event callbacks
        using ResizeCallback = std::function<void(int width, int height)>;
        using CloseCallback = std::function<void()>;
        using KeyCallback = std::function<void(int key, int action)>;

        void SetResizeCallback(ResizeCallback callback) { resizeCallback = callback; }
        void SetCloseCallback(CloseCallback callback) { closeCallback = callback; }
        void SetKeyCallback(KeyCallback callback) { keyCallback = callback; }

        // Platform-specific getters
#ifdef _WIN32
        HWND GetHWND() const { return hwnd; }
        HDC GetHDC() const { return hdc; }
#endif

        // Rendering context setup (for OpenGL/Vulkan/DirectX)
        bool SetupRenderingContext(const std::string& api);
        void MakeContextCurrent();

    private:
        WindowProperties properties;
        bool isCreated;

        // Platform-specific handles
#ifdef _WIN32
        HWND hwnd;
        HDC hdc;
        HGLRC hglrc; // OpenGL context
        WNDCLASS windowClass;
        
        // VSync support
        typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int);
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
        bool vsyncSupported;
        bool currentVSyncState;
#endif

        // Callbacks
        ResizeCallback resizeCallback;
        CloseCallback closeCallback;
        KeyCallback keyCallback;

        // Platform-specific implementation
        bool CreatePlatformWindow();
        void DestroyPlatformWindow();
        void UpdateWindowProperties();

#ifdef _WIN32
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        void HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
    };
}
