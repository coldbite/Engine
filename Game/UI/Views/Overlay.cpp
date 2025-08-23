#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "Graphics/UI/Text/Text.h"

Overlay::Overlay() : Engine::View("Overlay") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    overlayText.SetValue("Game Menu");
    overlayText.SetFont("Sansation");
    overlayText.SetColor(text_color);
    overlayText.SetBackground(new Engine::Graphics::RGBA(0, 0, 0, 40));
    overlayText.SetPadding(10.0f, 5.0f);
    overlayText.SetMargin(30.0f, 0.0f, 0.0f, 60.0f);
    overlayText.SetSize(40.0f);
    overlayText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
}

void Overlay::OnShow() {}
void Overlay::OnHide() {}

void Overlay::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void) width;
    (void) oldWidth;
    overlayText.UpdateFontSizeForWindow(height, 40, oldHeight);
}

void Overlay::OnUpdate(float deltaTime) {
    (void) deltaTime;
}


void Overlay::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Begin2D(context.GetWidth(), context.GetHeight());
    context.DrawRect(0, 0, context.GetWidth(), context.GetHeight(), GetBackground());

    overlayText.Render(context, Engine::Graphics::TextAlignment::TOP_LEFT);

    context.End2D();
}

