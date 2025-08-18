#pragma once

#include "../../../Engine/Core/View.h"
#include <string>

class Loading : public Engine::View {
public:
    Loading();

    // View lifecycle
    void OnShow() override;
    void OnHide() override;
    void OnUpdate(float deltaTime) override;

    // Loading progress
    void UpdateProgress(const std::string& message, int actual, int total, float percentage);

protected:
    void Render() override;

private:
    std::string currentMessage;
    int currentStep;
    int totalSteps;
    float progressPercentage;

    void DisplayProgress();
};
