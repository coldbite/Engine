#pragma once

#include "../IRenderingAPI.h"
#include <string>
#include <memory>
#include <map>

#ifdef _WIN32
    #include <windows.h>
    #include <wingdi.h>
    #include <gl/gl.h>
#endif

namespace Engine {
    class NativeWindow;

    namespace Graphics {
        namespace OpenGL {

            class OpenGL : public IRenderingAPI {
            public:
                virtual ~OpenGL();
                bool Init(std::shared_ptr<NativeWindow> window) override;

                // Implementations for the pure virtual functions
                void CreateDevice() override;
                void GetDevice() override;
                void CreateContext() override;
                void GetContext() override;
                void DrawText(const std::string& text, float x, float y, float r = 1.0f, float g = 1.0f, float b = 1.0f) override;

                static void Shutdown();
                static void MakeContextCurrent();

                // Basic rendering functions
                static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
                static void SwapBuffers();

                // 2D rendering for UI
                static void Begin2D(int width, int height);
                static void End2D();

                // Primitive drawing
                static void DrawRect(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
                static void DrawSpinner(float x, float y, float radius, float rotation);

            private:
                static std::shared_ptr<NativeWindow> currentWindow;
                static bool initialized;
            };
        }
    }
}
