#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/UI/Text/Text.h"

class Overlay;

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

        void ToggleOverlay();

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::IColor* text_color       = new Engine::Graphics::HEX("#444444");
        Engine::Graphics::IColor* color_background = new Engine::Graphics::HEX("#AAAAAA");


        // Keep reference to overlay to avoid recreation
        std::shared_ptr<Overlay> gameOverlay;
        bool overlayVisible = false;

        // Debounce system to prevent rapid toggling
        static constexpr float TOGGLE_DEBOUNCE_TIME = 0.3f; // 300ms
        float lastToggleTime = 0.0f;
};
