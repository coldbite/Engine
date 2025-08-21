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

                virtual bool Available() = 0;
                virtual std::string GetVersion() = 0;
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

                // Texture drawing
                virtual void DrawTexture(const Texture& texture, float x, float y, float width, float height) = 0;
                virtual void DrawTextureBlurred(const Texture& texture, float x, float y, float width, float height, float blurRadius = 2.0f) = 0;
                
                // Overlay effects
                virtual void DrawDiagonalLines(float x, float y, float width, float height, float lineSpacing = 10.0f, float lineWidth = 1.0f, const IColor& color = RGBA(1.0f, 1.0f, 1.0f, 0.1f)) = 0;
                virtual void DrawRadialLines(float x, float y, float width, float height, float centerX1, float centerY1, float centerX2, float centerY2, int numLines = 20, float lineWidth = 1.0f, const IColor& color = RGBA(1.0f, 1.0f, 1.0f, 0.1f)) = 0;
                virtual void DrawVerticalLines(float x, float y, float width, float height, float lineSpacing = 15.0f, float lineWidth = 1.0f, const IColor& color = RGBA(1.0f, 1.0f, 1.0f, 0.08f)) = 0;
                virtual void DrawHorizontalLines(float x, float y, float width, float height, float lineSpacing = 15.0f, float lineWidth = 1.0f, const IColor& color = RGBA(1.0f, 1.0f, 1.0f, 0.08f)) = 0;
                virtual void DrawFilmGrain(float x, float y, float width, float height, float intensity = 0.08f, int seed = 0) = 0;

                // Primitive drawing
                virtual void DrawRect(float x, float y, float width, float height, const IColor& color) = 0;
                virtual void PaintText(const std::string& text, float x, float y, const IColor& color) = 0;
        };
    }
}
