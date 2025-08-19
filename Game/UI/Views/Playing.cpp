#include "Playing.h"
#include "../../../Engine/Graphics/OpenGL/OpenGL.h"
#include <iostream>
#include "../../../Engine/Graphics/RGBA.h"
#include "../../../Engine/Graphics/Text.h"

Engine::Graphics::Text text;

Playing::Playing()
    : Engine::View("Playing")
    , hudInfo("Game Ready")
    , lastNotification("")
    , notificationTimer(0.0f) {
    /* Do Nothing */


    text.LoadFont("C:/Windows/Fonts/arial.ttf", 22);
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
    using namespace Engine::Graphics::OpenGL;

    OpenGL::Clear(0.0f, 0.0f, 0.0f, 1.0f);

    text.RenderText("Hello World!", 100, 100, 1.0f, Engine::Graphics::RGBA(1.0f, 0.0f, 0.0f, 0.5f));

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
