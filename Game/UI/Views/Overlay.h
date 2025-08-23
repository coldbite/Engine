#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/UI/Text/Text.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Overlay : public Engine::View {
    public:
        Overlay();

        void OnShow() override;
        void OnHide() override;
        void OnUpdate(float deltaTime) override;
        void OnResize(int width, int height, int oldWidth, int oldHeight) override;

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text overlayText;
        Engine::Graphics::IColor* text_color        = new Engine::Graphics::HEX("#FFFFFF");
        Engine::Graphics::IColor* color_background  = new Engine::Graphics::RGBA(1.0f, 0.0f, 0.0f, 0.3f);
};
