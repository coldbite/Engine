#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"

Overlay::Overlay() : Engine::View("Overlay") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    overlayText.SetValue("Test");
    overlayText.SetFont("Sansation");
    overlayText.SetSize(22.0f);
    overlayText.SetColor(text_color);
    overlayText.SetBackground(new Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.3f));
}

void Overlay::OnShow() {}
void Overlay::OnHide() {}

void Overlay::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void) width;
    (void) oldWidth;
    overlayText.UpdateFontSizeForWindow(height, 22, oldHeight);
}

void Overlay::OnUpdate(float deltaTime) {
    (void) deltaTime;
}


void Overlay::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Begin2D(context.GetWidth(), context.GetHeight());
    context.DrawRect(0, 0, context.GetWidth(), context.GetHeight(), GetBackground());

    overlayText.Render(context, Engine::Graphics::TextAlignment::CENTER);

    context.End2D();
}

