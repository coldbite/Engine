#include "Playing.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Engine::Graphics::Text text;

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    text.LoadFont("../Game/Assets/Fonts/Sansation-Regular.ttf", 22);
}

void Playing::OnShow() {
    std::cout << "[Playing] View shown" << std::endl;
}

void Playing::OnHide() {
    std::cout << "[Playing] View hidden" << std::endl;
}

void Playing::OnResize(int width, int height, int oldWidth, int oldHeight) {
    // Resize text to maintain relative size
    text.UpdateFontSizeForWindow(height, 22, oldHeight);
}

void Playing::OnUpdate(float deltaTime) {
    // Currently no internal updates needed for Playing view
    // UpdateInternal(deltaTime) would be called here if we had animations or game logic
}

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    context.Begin2D(context.GetWidth(), context.GetHeight());
    text.RenderText(context, "Playing game...!", 10.0f, 10.0f, text_color);
    context.End2D();
}

