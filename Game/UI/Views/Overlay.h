#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/UI/Text/Text.h"
#include "../../../Engine/Graphics/UI/Menu/Menu.h"
#include "Contents/Score.h"
#include "Contents/Assignments.h"
#include "Contents/Options.h"
#include "Contents/ServerInfo.h"
#include <string>
#include <memory>

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
        
        // Mouse event handling
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseUp(float x, float y);
        void OnMouseScroll(float x, float y);

    protected:
        void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

    private:
        Engine::Graphics::Text overlayText;
        Engine::Graphics::Menu gameMenu;
        Engine::Graphics::IColor* text_color        = new Engine::Graphics::HEX("#FFFFFF");
        Engine::Graphics::IColor* color_background  = new Engine::Graphics::RGBA(0, 0, 0, 50);
        
        // Content area state
        std::string currentContent;
        float contentScrollY;
        
        // Content instances
        std::unique_ptr<Score> scoreContent;
        std::unique_ptr<Assignments> assignmentsContent;
        std::unique_ptr<Options> optionsContent;
        std::unique_ptr<ServerInfo> serverInfoContent;
        
        void ShowContent(const std::string& contentType);
        void CloseOverlay();
};
