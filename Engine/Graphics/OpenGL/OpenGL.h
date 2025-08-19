#pragma once

#include "../IRenderingAPI.h"
#include <string>
#include <memory>

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
                
                // Rendering operations
                void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) override;
                void SwapBuffers() override;

                // 2D rendering setup
                void Begin2D(int width, int height) override;
                void End2D() override;

                // Primitive drawing
                void DrawRect(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) override;
                void DrawSpinner(float x, float y, float radius, float rotation) override;
                void DrawTextString(const std::string& text, float x, float y, const IColor& color) override;

                // Static methods for backwards compatibility
                static void Shutdown();
                static void MakeContextCurrent();
                static void ClearStatic(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
                static void SwapBuffersStatic();
                static void Begin2DStatic(int width, int height);
                static void End2DStatic();
                static void DrawRectStatic(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
                static void DrawSpinnerStatic(float x, float y, float radius, float rotation);

            private:
                static std::shared_ptr<NativeWindow> currentWindow;
                static bool initialized;
            };
        }
    }
}
