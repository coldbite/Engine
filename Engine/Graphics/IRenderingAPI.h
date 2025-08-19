#pragma once
#include "IColor.h"
#include <string>
#include <memory>

namespace Engine {
    class NativeWindow;
    
    namespace Graphics {
        class IRenderingAPI {
            public:
                virtual ~IRenderingAPI() {}
                virtual bool Init(std::shared_ptr<NativeWindow> window) = 0;

                virtual void CreateDevice() = 0;
                virtual void GetDevice() = 0;
                virtual void CreateContext() = 0;
                virtual void GetContext() = 0;

                // Rendering operations
                virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
                virtual void SwapBuffers() = 0;

                // 2D rendering setup
                virtual void Begin2D(int width, int height) = 0;
                virtual void End2D() = 0;

                // Primitive drawing
                virtual void DrawRect(float x, float y, float width, float height, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) = 0;
                virtual void DrawSpinner(float x, float y, float radius, float rotation) = 0;
                virtual void DrawTextString(const std::string& text, float x, float y, const IColor& color) = 0;
        };
    }
}
