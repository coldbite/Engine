#include "Playing.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Engine::Graphics::Text text;

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    text.LoadFont("../Game/Assets/Fonts/Sansation-Regular.ttf", 18);
}

void Playing::OnShow() {
    std::cout << "[Playing] View shown" << std::endl;
}

void Playing::OnHide() {
    std::cout << "[Playing] View hidden" << std::endl;
}

void Playing::OnResize(int width, int height, int oldWidth, int oldHeight) {
    // Resize text to maintain relative size
    text.UpdateFontSizeForWindow(height, 18, oldHeight);
}

void Playing::OnUpdate(float deltaTime) {
    // Currently no internal updates needed for Playing view
    // UpdateInternal(deltaTime) would be called here if we had animations or game logic
}

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    // Set up 2D rendering with current window dimensions
    context.Begin2D(GetWindowWidth(), GetWindowHeight());

    // Use centered positioning (maintains aspect ratio)
    float textX = GetCenteredX(100.0f);
    float textY = GetCenteredY(100.0f);

    text.RenderText(context, "Playing game...!", textX, textY, text_color);

    context.End2D();
}

