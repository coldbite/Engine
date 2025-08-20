#pragma once
#include "../../../Engine/Core/View.h"
#include "../../../Engine/Graphics/Text.h"
#include <string>
#include <chrono>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Loading : public Engine::View {
public:
    Loading();

    // View lifecycle
    void OnShow() override;
    void OnHide() override;
    void OnUpdate(float deltaTime) override;
    void OnResize(int width, int height, int oldWidth, int oldHeight) override;

    // Loading progress
    void UpdateProgress(const std::string& message, int actual, int total, float percentage);

    // Game info
    void SetMapName(const std::string& mapName) { this->mapName = mapName; }
    void SetGameMode(const std::string& gameMode) { this->gameMode = gameMode; }

protected:
    void Render(Engine::Graphics::IRenderingAPI& renderingAPI) override;

private:
    std::string currentMessage;
    int currentStep;
    int totalSteps;
    float progressPercentage;

    Engine::Graphics::Text title;
    Engine::Graphics::Text text1;

    // Display info
    std::string mapName = "Unknown Map";
    std::string gameMode = "Singleplayer";
    Engine::Graphics::HEX color_background = Engine::Graphics::HEX("#0F0E12");

    // Animation timing
    std::chrono::steady_clock::time_point startTime;
    float animationCycleDuration = 3.4f; // Total animation cycle in seconds

    // Animation helper methods
    float GetAnimationTime() const;
    float GetTitleAlpha(float animTime) const;
    float GetTextAlpha(float animTime) const;
    float GetTitleX(float animTime) const;
    bool GetCursorVisible(float animTime) const;
    Engine::Graphics::RGBA GetAnimatedTextShadow(float animTime) const;

    void DisplayProgress();
    void RenderSpinner(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float rotation);
    void RenderProgressBar(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float width, float height, float progress);
    void RenderHeader(Engine::Graphics::IRenderingAPI& renderingAPI);
    void RenderAnimatedText(Engine::Graphics::IRenderingAPI& renderingAPI, int windowWidth, int windowHeight);
};
