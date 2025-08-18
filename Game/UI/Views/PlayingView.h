#pragma once

#include "../../../Engine/Core/UIView.h"
#include <string>

class PlayingView : public Engine::UIView {
public:
    PlayingView();
    
    // View lifecycle
    void OnShow() override;
    void OnHide() override;
    void OnUpdate(float deltaTime) override;
    
    // Game UI methods
    void UpdateHUD(const std::string& info);
    void ShowNotification(const std::string& message);
    
protected:
    void RenderInternal() override;
    
private:
    std::string hudInfo;
    std::string lastNotification;
    float notificationTimer;
    
    void DisplayHUD();
    void UpdateNotifications(float deltaTime);
};