#pragma once
#include "IColor.h"
#include "RGBA.h"
#include "Texture.h"
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

                virtual void SetViewport(int width, int height) = 0;
                virtual int GetWidth() = 0;
                virtual int GetHeight() = 0;

                // Rendering operations
                virtual void Clear() = 0;
                virtual void Clear(const IColor& color) = 0;
                virtual void SwapBuffers() = 0;

                // 2D rendering setup
                virtual void Begin2D(int width, int height) = 0;
                virtual void End2D() = 0;

                virtual Texture LoadTexture(const std::string& path) = 0;

                // Primitive drawing
                virtual void DrawRect(float x, float y, float width, float height, const IColor& color) = 0;
                virtual void PaintText(const std::string& text, float x, float y, const IColor& color) = 0;
        };
    }
}
