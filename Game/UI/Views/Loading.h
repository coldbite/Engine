#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/Text.h"
#include "../../../Engine/Graphics/RGBA.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Loading : public Engine::View {
    public:
        Loading();

        void OnShow() override;
        void OnHide() override;
        void OnUpdate(float deltaTime) override;
        void OnResize(int width, int height, int oldWidth, int oldHeight) override;

        void OnUpdateProgress(const std::string& message, int actual, int total, float percentage);
        void SetMapName(const std::string& name) { this->mapName = name; }
        void SetGameMode(const std::string& mode) { this->gameMode = mode; }

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text text_map;
        Engine::Graphics::Text text_mode;
        Engine::Graphics::HEX text_color       = Engine::Graphics::HEX("#FEFEFE");
        Engine::Graphics::HEX color_background = Engine::Graphics::HEX("#333333");

        std::string mapName     = "MAP";
        std::string gameMode    = "MODE";
};
