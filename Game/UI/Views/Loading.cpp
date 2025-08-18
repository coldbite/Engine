#include "Loading.h"
#include "../../../Engine/Graphics/OpenGL/OpenGL.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Loading::Loading()
    : Engine::View("Loading")
    , currentMessage("Initializing...")
    , currentStep(0)
    , totalSteps(0)
    , progressPercentage(0.0f)
    , startTime(std::chrono::steady_clock::now()) {
    /* Do Nothing */
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

void Loading::Render() {
    std::cout << "[Loading] Render() called!" << std::endl;
    // Call OpenGL rendering
    RenderOpenGL();
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

void Loading::RenderOpenGL() {
    std::cout << "[Loading] RenderOpenGL() called!" << std::endl;
    using namespace Engine::Graphics::OpenGL;

    // Clear screen with dark background
    OpenGL::Clear(0.0f, 0.0f, 0.0f, 1.0f);

    // Set up 2D rendering
    OpenGL::Begin2D(1024, 768); // Use standard resolution for now

    // Render header
    RenderHeader();

    // Render status text (bottom left)
    RenderText(currentMessage, 50, 700);

    // Render spinner (bottom right)
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
    float rotation = (elapsed.count() / 100.0f) * 0.1f; // Slow rotation

    RenderSpinner(950, 720, rotation);

    // Render progress bar if we have progress
    if (progressPercentage > 0) {
        RenderProgressBar(50, 650, 300, 20, progressPercentage / 100.0f);
    }

    OpenGL::End2D();

    // Note: ViewManager will handle SwapBuffers()
}

void Loading::RenderHeader() {
    using namespace Engine::Graphics::OpenGL;

    // Header background
    OpenGL::DrawRect(0, 0, 1024, 80, 0.2f, 0.2f, 0.3f, 0.9f);

    // Game mode (top left)
    RenderText("Mode: " + gameMode, 20, 25);

    // Map name (top center)
    RenderText("Loading: " + mapName, 400, 25);

    // Version/title (top right)
    RenderText("Masterball Beta", 850, 25);
}

void Loading::RenderSpinner(float x, float y, float rotation) {
    using namespace Engine::Graphics::OpenGL;

    OpenGL::DrawSpinner(x, y, 20, rotation);
}

void Loading::RenderProgressBar(float x, float y, float width, float height, float progress) {
    using namespace Engine::Graphics::OpenGL;

    // Background
    OpenGL::DrawRect(x, y, width, height, 0.3f, 0.3f, 0.3f, 0.8f);

    // Progress fill
    if (progress > 0) {
        OpenGL::DrawRect(x + 2, y + 2, (width - 4) * progress, height - 4, 0.4f, 0.8f, 0.4f, 0.9f);
    }

    // Border
    OpenGL::DrawRect(x, y, width, 2, 0.6f, 0.6f, 0.6f, 1.0f); // Top
    OpenGL::DrawRect(x, y + height - 2, width, 2, 0.6f, 0.6f, 0.6f, 1.0f); // Bottom
    OpenGL::DrawRect(x, y, 2, height, 0.6f, 0.6f, 0.6f, 1.0f); // Left
    OpenGL::DrawRect(x + width - 2, y, 2, height, 0.6f, 0.6f, 0.6f, 1.0f); // Right

    // Progress text
    std::string progressText = std::to_string(static_cast<int>(progress * 100)) + "%";
    RenderText(progressText, x + width + 10, y + 5);
}

void Loading::RenderText(const std::string& text, float x, float y) {
    using namespace Engine::Graphics::OpenGL;

    std::cout << "[Loading] RenderText called with: '" << text << "'" << std::endl;
    OpenGL::DrawText(text, x, y, 1.0f, 1.0f, 1.0f);
}
