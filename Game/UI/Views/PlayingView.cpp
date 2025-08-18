#include "PlayingView.h"
#include <iostream>

PlayingView::PlayingView() 
    : Engine::UIView("PlayingView")
    , hudInfo("Game Ready")
    , lastNotification("")
    , notificationTimer(0.0f) {
}

void PlayingView::OnShow() {
    std::cout << "[PlayingView] === GAME INTERFACE ACTIVE ===" << std::endl;
    std::cout << "[PlayingView] Welcome to Masterball!" << std::endl;
    DisplayHUD();
}

void PlayingView::OnHide() {
    std::cout << "[PlayingView] === GAME INTERFACE HIDDEN ===" << std::endl;
}

void PlayingView::OnUpdate(float deltaTime) {
    if (IsActive()) {
        UpdateNotifications(deltaTime);
        UpdateInternal(deltaTime);
    }
}

void PlayingView::UpdateHUD(const std::string& info) {
    hudInfo = info;
    if (IsActive()) {
        DisplayHUD();
    }
}

void PlayingView::ShowNotification(const std::string& message) {
    lastNotification = message;
    notificationTimer = 3.0f; // Show for 3 seconds
    
    if (IsActive()) {
        std::cout << "[PlayingView] NOTIFICATION: " << message << std::endl;
    }
}

void PlayingView::RenderInternal() {
    // Common rendering logic that applies to all APIs
    // Don't call DisplayHUD here every frame - it's too verbose
    // DisplayHUD is called when HUD actually updates
}

void PlayingView::RenderOpenGL() {
    // OpenGL-specific rendering for game UI
    // TODO: OpenGL calls for game UI
    // - Render HUD elements (health, ammo, minimap)
    // - Draw crosshair
    // - Render notifications/text overlays
    // (Silent rendering - no debug output)
}

void PlayingView::RenderDirectX() {
    // DirectX-specific rendering for game UI
    std::cout << "[PlayingView] Rendering HUD with DirectX" << std::endl;
    // TODO: DirectX calls for game UI
    // - Render UI sprites and textures
    // - Draw HUD elements with DirectX primitives
    // - Render text with DirectWrite
}

void PlayingView::RenderVulkan() {
    // Vulkan-specific rendering for game UI
    std::cout << "[PlayingView] Rendering HUD with Vulkan" << std::endl;
    // TODO: Vulkan calls for game UI
    // - Begin UI render pass
    // - Draw HUD with Vulkan commands
    // - Render text and UI elements
}

void PlayingView::DisplayHUD() {
    // Only display HUD when it's actually called, not every frame
    std::cout << "[PlayingView] HUD: " << hudInfo << std::endl;
    
    // Display current notification if active (only when first shown)
    if (notificationTimer > 0 && !lastNotification.empty()) {
        std::cout << "[PlayingView] >>> " << lastNotification << " <<<" << std::endl;
    }
}

void PlayingView::UpdateNotifications(float deltaTime) {
    if (notificationTimer > 0) {
        notificationTimer -= deltaTime;
        if (notificationTimer <= 0) {
            lastNotification.clear();
        }
    }
}