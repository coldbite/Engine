#include "Loading.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Graphics/Effects/AllEffects.h"
#include "Graphics/Effects/FadeEffect.h"
#include "Graphics/Effects/TypewriterEffect.h"
#include <iostream>

Loading::Loading() : Engine::View("Loading") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    /* Text: Map */
    text_map.SetValue(this->mapName);
    text_map.SetFont("Sansation");
    text_map.SetColor(text_color);
    text_map.SetBackground(Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f));
    text_map.SetPadding(10.0f, 5.0f);
    text_map.SetSize(40.0f);
    text_map.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);

    /* Text: Mode */
    text_mode.SetValue(this->gameMode);
    text_mode.SetFont("Sansation");
    text_mode.SetColor(text_color);
    text_mode.SetBackground(Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f));
    text_mode.SetPadding(10.0f, 5.0f);
    text_mode.SetSize(20.0f);
    text_mode.SetStyle(Engine::Graphics::FontStyle::UPPERCASE);

    /* Text: Loading */
    text_status.SetValue("Loading");
    text_status.SetFont("Sansation");
    text_status.SetColor(text_color);
    text_status.SetSize(20.0f);
    text_status.SetMargin(0.0f, 30.0f, 20.0f, 0.0f);
    text_status.SetStyle(Engine::Graphics::FontStyle::UPPERCASE);
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateAmbience(0.8f));
}

void Loading::OnShow() {}
void Loading::OnHide() {}
void Loading::OnResize(int width, int height, int oldWidth, int oldHeight) {}

void Loading::OnUpdate(float deltaTime) {
    text_status.Update(deltaTime);
    text_map.SetValue(this->mapName);
    text_mode.SetValue(this->gameMode);
}

void Loading::OnUpdateProgress(const std::string& message, int actual, int total, float percentage) {
    text_status.SetValue(message);
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateFadeIn(0.3f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateSlideFromLeft(150.0f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateFadeOut(5.0f));
}

void Loading::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    float w = static_cast<float>(context.GetWidth());
    float h = static_cast<float>(context.GetHeight());
    context.Begin2D(context.GetWidth(), context.GetHeight());

    /* Background */
    std::string file = GetBackgroundImage();

    if(!file.empty()) {
        static auto texture = context.LoadTexture("../Game/" + file);

        context.DrawTextureBlurred(texture, 0.0f, 1.0f, w, h, 5.0f);
        context.DrawHorizontalLines(0.0f, 0.0f, w, h, 3.0f, 4.0f, Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.3f));
        context.DrawRect(0.0f, 0.0f, w, h, Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.2f));
        context.DrawFilmGrain(0.0f, 0.0f, w, h, 0.15f, 123);
    }

    text_map.Render(context, GetScaledX(20.0f), GetScaledY(30.0f));
    text_mode.Render(context, GetScaledX(20.0f), GetScaledY(82.0f));
    text_status.Render(context, Engine::Graphics::TextAlignment::BOTTOM_RIGHT);

    context.End2D();
}
