#include "LoadingView.h"
#include <iostream>
#include <iomanip>

LoadingView::LoadingView() 
    : Engine::UIView("LoadingView")
    , currentMessage("Initializing...")
    , currentStep(0)
    , totalSteps(0)
    , progressPercentage(0.0f) {
}

void LoadingView::OnShow() {
    std::cout << "[LoadingView] === LOADING SCREEN SHOWN ===" << std::endl;
    std::cout << "[LoadingView] Please wait while the game loads..." << std::endl;
    DisplayProgress();
}

void LoadingView::OnHide() {
    std::cout << "[LoadingView] === LOADING COMPLETED ===" << std::endl;
}

void LoadingView::OnUpdate(float deltaTime) {
    // Could animate loading spinner, update dots, etc.
    // For now, just update if we have progress to show
    if (IsActive()) {
        UpdateInternal(deltaTime);
    }
}

void LoadingView::UpdateProgress(const std::string& message, int actual, int total, float percentage) {
    currentMessage = message;
    currentStep = actual;
    totalSteps = total;
    progressPercentage = percentage;
    
    if (IsActive()) {
        DisplayProgress();
    }
}

void LoadingView::RenderInternal() {
    // This is where actual rendering would happen (OpenGL/Vulkan/DirectX)
    // For now, just ensure our display is current
    DisplayProgress();
}

void LoadingView::DisplayProgress() {
    std::cout << "[LoadingView] " << currentMessage;
    
    if (totalSteps > 0) {
        std::cout << " [" << currentStep << "/" << totalSteps << "]";
    }
    
    if (progressPercentage > 0) {
        std::cout << " " << std::fixed << std::setprecision(1) << progressPercentage << "%";
    }
    
    std::cout << std::endl;
    
    // Simple ASCII progress bar
    if (progressPercentage > 0) {
        std::cout << "[LoadingView] [";
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