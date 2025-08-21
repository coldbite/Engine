#include "Playing.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Engine::Graphics::Text text;

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    text.SetValue("Playing game...!");
    text.SetFont("Sansation");
    text.SetSize(22.0f);
    text.SetColor(text_color);
}

void Playing::OnShow() {
    std::cout << "[Playing] View shown" << std::endl;
}

void Playing::OnHide() {
    std::cout << "[Playing] View hidden" << std::endl;
}

void Playing::OnResize(int width, int height, int oldWidth, int oldHeight) {
    text.UpdateFontSizeForWindow(height, 22, oldHeight);
}

void Playing::OnUpdate(float deltaTime) {}

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());
    context.Begin2D(context.GetWidth(), context.GetHeight());
    text.Render(context, Engine::Graphics::TextAlignment::CENTER);
    context.End2D();
}

