#include "Loading.h"
#include "../../../Engine/Graphics/OpenGL/OpenGL.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include "../../../Engine/Graphics/Text.h"

Engine::Graphics::Text text1;

Loading::Loading()
    : Engine::View("Loading")
    , currentMessage("Initializing...")
    , currentStep(0)
    , totalSteps(0)
    , progressPercentage(0.0f)
    , startTime(std::chrono::steady_clock::now()) {
    /* Do Nothing */

    text1.LoadFont("C:/Windows/Fonts/arial.ttf", 18);
}

void Loading::OnShow() {
    DisplayProgress();
}

void Loading::OnHide() {
    /* Do Nothing */
}

void Loading::OnUpdate(float deltaTime) {
    // Could animate loading spinner, update dots, etc.
    // For now, just update if we have progress to show
    if(IsActive()) {
        UpdateInternal(deltaTime);
    }
}

void Loading::UpdateProgress(const std::string& message, int actual, int total, float percentage) {
    currentMessage      = message;
    currentStep         = actual;
    totalSteps          = total;
    progressPercentage  = percentage;

    // Only display progress when it's actually updated from the loading thread
    // Not during regular rendering

    if(IsActive()) {
        DisplayProgress();
    }
}

void Loading::Render(Engine::Graphics::IRenderingAPI& renderingAPI) {
    // Clear screen with dark background
    renderingAPI.Clear(0.0f, 0.0f, 0.0f, 1.0f);

    // Set up 2D rendering
    int windowWidth = 1024;
    int windowHeight = 768;
    renderingAPI.Begin2D(windowWidth, windowHeight);

    // Render header
    RenderHeader(renderingAPI);

    // Render status text (bottom left)
    text1.RenderText(renderingAPI, currentMessage, 50, 100, 1.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 0.0f, 1.0f));

    // Render spinner (bottom right)
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
    float rotation = (elapsed.count() / 100.0f) * 0.1f; // Slow rotation

    RenderSpinner(renderingAPI, 950, 720, rotation);

    // Render progress bar if we have progress
    if (progressPercentage > 0) {
        RenderProgressBar(renderingAPI, 50, 650, 300, 20, progressPercentage / 100.0f);
    }

    renderingAPI.End2D();
}

void Loading::DisplayProgress() {
    std::cout << "[Loading] " << currentMessage;

    if(totalSteps > 0) {
        std::cout << " [" << currentStep << "/" << totalSteps << "]";
    }

    if(progressPercentage > 0) {
        std::cout << " " << std::fixed << std::setprecision(1) << progressPercentage << "%";
    }

    std::cout << std::endl;

    // Simple ASCII progress bar
    if(progressPercentage > 0) {
        std::cout << "[Loading] [";
        int barWidth = 30;
        int progress = static_cast<int>(progressPercentage / 100.0f * barWidth);

        for(int i = 0; i < barWidth; ++i) {
            if(i < progress) {
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






void Loading::RenderHeader(Engine::Graphics::IRenderingAPI& context) {
    // Header background
    context.DrawRect(0, 0, 1024, 80, 0.2f, 0.2f, 0.3f, 0.9f);

    // Text rendering using IRenderingAPI abstraction
    text1.RenderText(context, "Mode: " + gameMode, 20, 550, 1.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 0.0f, 1.0f));
    text1.RenderText(context, "Loading: " + mapName, 400, 550, 1.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 0.0f, 1.0f));
    text1.RenderText(context, "Masterball Beta", 850, 550, 1.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 0.0f, 1.0f));
}

void Loading::RenderSpinner(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float rotation) {
    renderingAPI.DrawSpinner(x, y, 20, rotation);
}

void Loading::RenderProgressBar(Engine::Graphics::IRenderingAPI& renderingAPI, float x, float y, float width, float height, float progress) {
    // Background
    renderingAPI.DrawRect(x, y, width, height, 0.3f, 0.3f, 0.3f, 0.8f);

    // Progress fill
    if (progress > 0) {
        renderingAPI.DrawRect(x + 2, y + 2, (width - 4) * progress, height - 4, 0.4f, 0.8f, 0.4f, 0.9f);
    }

    // Border
    renderingAPI.DrawRect(x, y, width, 2, 0.6f, 0.6f, 0.6f, 1.0f); // Top
    renderingAPI.DrawRect(x, y + height - 2, width, 2, 0.6f, 0.6f, 0.6f, 1.0f); // Bottom
    renderingAPI.DrawRect(x, y, 2, height, 0.6f, 0.6f, 0.6f, 1.0f); // Left
    renderingAPI.DrawRect(x + width - 2, y, 2, height, 0.6f, 0.6f, 0.6f, 1.0f); // Right

    // Progress text
    std::string progressText = std::to_string(static_cast<int>(progress * 100)) + "%";
    text1.RenderText(renderingAPI, progressText, x + width + 10, 600 - (y + 5), 1.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 0.0f, 1.0f));
}
