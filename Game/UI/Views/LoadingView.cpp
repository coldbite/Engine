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
    
    // Only display progress when it's actually updated from the loading thread
    // Not during regular rendering
    if (IsActive()) {
        DisplayProgress();
    }
}

void LoadingView::RenderInternal() {
    // Common rendering logic that applies to all APIs
    // Don't call DisplayProgress here as it's called from UpdateProgress
}

void LoadingView::RenderOpenGL() {
    // OpenGL-specific rendering for loading screen
    // TODO: OpenGL calls for loading screen UI
    // - Clear screen
    // - Draw progress bar geometry
    // - Render text
    // (Silent rendering - no debug output)
}

void LoadingView::RenderDirectX() {
    // DirectX-specific rendering for loading screen
    std::cout << "[LoadingView] Rendering with DirectX" << std::endl;
    // TODO: DirectX calls for loading screen UI
    // - Clear render target
    // - Draw progress bar with DirectX primitives
    // - Render text with DirectWrite
}

void LoadingView::RenderVulkan() {
    // Vulkan-specific rendering for loading screen
    std::cout << "[LoadingView] Rendering with Vulkan" << std::endl;
    // TODO: Vulkan calls for loading screen UI
    // - Begin render pass
    // - Draw progress bar with Vulkan commands
    // - Render text
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