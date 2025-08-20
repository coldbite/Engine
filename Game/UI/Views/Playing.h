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
        void OnResize(int width, int height, int oldWidth, int oldHeight) override;

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text text;
        Engine::Graphics::HEX text_color       = Engine::Graphics::HEX("#444444");
        Engine::Graphics::HEX color_background = Engine::Graphics::HEX("#AAAAAA");
};
