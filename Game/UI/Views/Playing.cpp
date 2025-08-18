#include "Playing.h"
#include <iostream>

Playing::Playing()
    : Engine::View("Playing")
    , hudInfo("Game Ready")
    , lastNotification("")
    , notificationTimer(0.0f) {
}

void Playing::OnShow() {
    std::cout << "[Playing] === GAME INTERFACE ACTIVE ===" << std::endl;
    std::cout << "[Playing] Welcome to Masterball!" << std::endl;
    DisplayHUD();
}

void Playing::OnHide() {
    std::cout << "[Playing] === GAME INTERFACE HIDDEN ===" << std::endl;
}

void Playing::OnUpdate(float deltaTime) {
    if (IsActive()) {
        UpdateNotifications(deltaTime);
        UpdateInternal(deltaTime);
    }
}

void Playing::UpdateHUD(const std::string& info) {
    hudInfo = info;
    if (IsActive()) {
        DisplayHUD();
    }
}

void Playing::ShowNotification(const std::string& message) {
    lastNotification = message;
    notificationTimer = 3.0f; // Show for 3 seconds

    if (IsActive()) {
        std::cout << "[Playing] NOTIFICATION: " << message << std::endl;
    }
}


void Playing::Render() {
    std::cout << "[Playing] Rendering" << std::endl;
}

void Playing::DisplayHUD() {
    // Only display HUD when it's actually called, not every frame
    std::cout << "[Playing] HUD: " << hudInfo << std::endl;

    // Display current notification if active (only when first shown)
    if (notificationTimer > 0 && !lastNotification.empty()) {
        std::cout << "[Playing] >>> " << lastNotification << " <<<" << std::endl;
    }
}

void Playing::UpdateNotifications(float deltaTime) {
    if (notificationTimer > 0) {
        notificationTimer -= deltaTime;
        if (notificationTimer <= 0) {
            lastNotification.clear();
        }
    }
}
