#pragma once

#include "../../../Engine/Core/View.h"
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
    
    // Display info
    std::string mapName = "Unknown Map";
    std::string gameMode = "Singleplayer";
    
    // Animation
    std::chrono::steady_clock::time_point startTime;
    
    void DisplayProgress();
    void RenderSpinner(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float rotation);
    void RenderProgressBar(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float width, float height, float progress);
    void RenderHeader(Engine::Graphics::IRenderingAPI& renderingAPI);
};
