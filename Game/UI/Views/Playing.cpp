#include "Playing.h"
#include <iostream>

Playing::Playing()
    : Engine::View("Playing")
    , hudInfo("Game Ready")
    , lastNotification("")
    , notificationTimer(0.0f) {
    /* Do Nothing */
}

void Playing::OnShow() {
    DisplayHUD();
}

void Playing::OnHide() {
    /* Do Nothing */
}

void Playing::OnUpdate(float deltaTime) {
    if(IsActive()) {
        UpdateNotifications(deltaTime);
        UpdateInternal(deltaTime);
    }
}

void Playing::UpdateHUD(const std::string& info) {
    hudInfo = info;

    if(IsActive()) {
        DisplayHUD();
    }
}

void Playing::ShowNotification(const std::string& message) {
    lastNotification = message;
    notificationTimer = 3.0f; // Show for 3 seconds

    if(IsActive()) {
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
    if(notificationTimer > 0 && !lastNotification.empty()) {
        std::cout << "[Playing] >>> " << lastNotification << " <<<" << std::endl;
    }
}

void Playing::UpdateNotifications(float deltaTime) {
    if(notificationTimer > 0) {
        notificationTimer -= deltaTime;

        if(notificationTimer <= 0) {
            lastNotification.clear();
        }
    }
}
