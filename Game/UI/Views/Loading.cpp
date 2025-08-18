#include "Loading.h"
#include <iostream>
#include <iomanip>

Loading::Loading()
    : Engine::View("Loading")
    , currentMessage("Initializing...")
    , currentStep(0)
    , totalSteps(0)
    , progressPercentage(0.0f) {
}

void Loading::OnShow() {
    std::cout << "[Loading] === LOADING SCREEN SHOWN ===" << std::endl;
    std::cout << "[Loading] Please wait while the game loads..." << std::endl;
    DisplayProgress();
}

void Loading::OnHide() {
    std::cout << "[Loading] === LOADING COMPLETED ===" << std::endl;
}

void Loading::OnUpdate(float deltaTime) {
    // Could animate loading spinner, update dots, etc.
    // For now, just update if we have progress to show
    if (IsActive()) {
        UpdateInternal(deltaTime);
    }
}

void Loading::UpdateProgress(const std::string& message, int actual, int total, float percentage) {
    currentMessage = message;
    currentStep = actual;
    totalSteps = total;
    progressPercentage = percentage;

    // Only display progress when it's actually updated from the loading thread
    // Not during regular rendering
    if (IsActive()) {
        DisplayProgress();
    }
}

void Loading::Render() {
    std::cout << "[Loading] RENDER..." << std::endl;
}

void Loading::DisplayProgress() {
    std::cout << "[Loading] " << currentMessage;

    if (totalSteps > 0) {
        std::cout << " [" << currentStep << "/" << totalSteps << "]";
    }

    if (progressPercentage > 0) {
        std::cout << " " << std::fixed << std::setprecision(1) << progressPercentage << "%";
    }

    std::cout << std::endl;

    // Simple ASCII progress bar
    if (progressPercentage > 0) {
        std::cout << "[Loading] [";
        int barWidth = 30;
        int progress = static_cast<int>(progressPercentage / 100.0f * barWidth);

        for (int i = 0; i < barWidth; ++i) {
            if (i < progress) {
                std::cout << "=";
            } else if (i == progress) {
                std::cout << ">";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "] " << std::fixed << std::setprecision(1) << progressPercentage << "%" << std::endl;
    }
}
