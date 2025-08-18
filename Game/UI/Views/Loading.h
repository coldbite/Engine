#pragma once

#include "../../../Engine/Core/View.h"
#include <string>
#include <chrono>

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
    void Render() override;

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
    void RenderOpenGL();
    void RenderSpinner(float x, float y, float rotation);
    void RenderProgressBar(float x, float y, float width, float height, float progress);
    void RenderText(const std::string& text, float x, float y);
    void RenderHeader();
};
