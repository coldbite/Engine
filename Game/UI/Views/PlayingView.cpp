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
    // This is where actual rendering would happen (OpenGL/Vulkan/DirectX)
    // HUD elements, menus, overlays, etc.
    DisplayHUD();
}

void PlayingView::DisplayHUD() {
    std::cout << "[PlayingView] HUD: " << hudInfo << std::endl;
    
    // Display current notification if active
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