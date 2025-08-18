#pragma once

#include "../../../Engine/Core/UIView.h"
#include <string>

class LoadingView : public Engine::UIView {
public:
    LoadingView();
    
    // View lifecycle
    void OnShow() override;
    void OnHide() override;
    void OnUpdate(float deltaTime) override;
    
    // Loading progress
    void UpdateProgress(const std::string& message, int actual, int total, float percentage);
    
protected:
    void RenderInternal() override;
    void RenderOpenGL() override;
    void RenderDirectX() override;
    void RenderVulkan() override;
    
private:
    std::string currentMessage;
    int currentStep;
    int totalSteps;
    float progressPercentage;
    
    void DisplayProgress();
};