#include "Shutdown.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Graphics/Effects/AllEffects.h"

Shutdown::Shutdown() : Engine::View("Shutdown") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    /* Text: Loading */
    text_status.SetValue("Exiting");
    text_status.SetFont("Sansation");
    text_status.SetColor(text_color);
    text_status.SetSize(20.0f);
    text_status.SetMargin(0.0f, 30.0f + 18.0f + 10.0f, 20.0f, 0.0f);
    text_status.SetStyle(Engine::Graphics::FontStyle::UPPERCASE);
    //text_status.SetShadow(10.0f, new Engine::Graphics::RGBA(188, 228, 255, 50), 0.0f, 0.0f);
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateAmbience(0.8f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateFadeIn(0.2f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateSlideFromLeft(100.0f, 0.3f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateFadeOut(5.0f));

    /* Box: Loader */
    box.SetSize(18, 18);
    box.SetMargin(0.0f, 30.0f, 24.0f, 0.0f);
    box.SetShadow(10.0f, new Engine::Graphics::RGBA(188, 228, 255, 50), 0.0f, 0.0f);
    box.SetColor(text_color);
    box.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateBlink(1.5f));
    box.GetAnimator().AddEffect(std::make_unique<Engine::Graphics::Effects::TravelEffect>(0.8f, 180.0f, 4.2f));
}

void Shutdown::OnShow() {}
void Shutdown::OnHide() {}
void Shutdown::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void) width; (void) height; (void) oldWidth; (void) oldHeight;
}

void Shutdown::OnUpdate(float deltaTime) {
    text_status.Update(deltaTime);
    box.Update(deltaTime);
}

void Shutdown::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    float w = static_cast<float>(context.GetWidth());
    float h = static_cast<float>(context.GetHeight());
    context.Begin2D(context.GetWidth(), context.GetHeight());

    /* Background */
    std::string file = GetBackgroundImage();

    if(!file.empty()) {
        static auto texture = context.LoadTexture("../Game/" + file);

        context.DrawTextureBlurred(texture, 0.0f, 1.0f, w, h, 5.0f);
        context.DrawHorizontalLines(0.0f, 0.0f, w, h, 3.0f, 4.0f, new Engine::Graphics::RGBA(0, 0, 0, 30));
        context.DrawRect(0.0f, 0.0f, w, h, new Engine::Graphics::RGBA(0, 0, 0, 20));
        context.DrawFilmGrain(0.0f, 0.0f, w, h, 0.15f, 123);
    }

    text_status.Render(context, Engine::Graphics::TextAlignment::BOTTOM_RIGHT);

    box.Render(context, Engine::Graphics::Alignment::BOTTOM_RIGHT);

    context.End2D();
}
