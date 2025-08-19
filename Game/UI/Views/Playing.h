#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/Text.h"
#include "../../../Engine/Graphics/RGBA.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Playing : public Engine::View {
    public:
        Playing();

        void OnShow() override;
        void OnHide() override;
        void OnUpdate(float deltaTime) override;
        void OnResize(int witdth, int height, int oldWidth, int oldHeight) override;

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text text;
        Engine::Graphics::RGBA text_color       = Engine::Graphics::RGBA(1.0f, 0.0f, 0.0f, 0.5f);
        Engine::Graphics::HEX color_background  = Engine::Graphics::HEX("#333333");
};
