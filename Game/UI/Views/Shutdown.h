#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/UI/Text/Text.h"
#include "../../../Engine/Graphics/UI/Box/Box.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Shutdown : public Engine::View {
    public:
        Shutdown();

        void OnShow() override;
        void OnHide() override;
        void OnUpdate(float deltaTime) override;
        void OnResize(int width, int height, int oldWidth, int oldHeight) override;

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text text_status;
        Engine::Graphics::Box box;
        Engine::Graphics::IColor* text_color       = new Engine::Graphics::HEX("#FEFEFE");
        Engine::Graphics::IColor* color_background = new Engine::Graphics::HEX("#000000");
};
