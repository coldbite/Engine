#pragma once

#include "../../../Engine/Core/View.h"
#include <string>

class Playing : public Engine::View {
public:
    Playing();

    // View lifecycle
    void OnShow() override;
    void OnHide() override;
    void OnUpdate(float deltaTime) override;

    // Game UI methods
    void UpdateHUD(const std::string& info);
    void ShowNotification(const std::string& message);

protected:
    void Render() override;

private:
    std::string hudInfo;
    std::string lastNotification;
    float notificationTimer;

    void DisplayHUD();
    void UpdateNotifications(float deltaTime);
};
