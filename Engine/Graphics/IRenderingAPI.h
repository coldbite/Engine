#pragma once
#include "NativeWindow.h"
#include "IColor.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI {
            public:
                virtual ~IRenderingAPI() {}
                virtual bool Init(std::shared_ptr<NativeWindow> window) = 0;

                virtual void CreateDevice() = 0;
                virtual void GetDevice() = 0;
                virtual void CreateContext() = 0;
                virtual void GetContext() = 0;

                virtual void DrawText(const std::string& text, float x, float y, const IColor& color) = 0;
        };
    }
}
