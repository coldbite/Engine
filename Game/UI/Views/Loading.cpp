#include "Loading.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Graphics/Effects/AllEffects.h"
#include "Graphics/Effects/TypewriterEffect.h"
#include <iostream>

Loading::Loading() : Engine::View("Loading") {
    SetBackground(color_background);

    // Set reference resolution for this view (1280x720 HD)
    SetReferenceResolution(1280.0f, 720.0f);

    text_map.SetValue(this->mapName);
    text_map.SetFont("Sansation");
    text_map.SetColor(text_color); // Red text
    text_map.SetBackground(Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f)); // Semi-transparent black background
    text_map.SetPadding(10.0f, 5.0f); // 10px horizontal, 5px vertical padding
    text_map.SetSize(40.0f);
    text_map.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);

    text_mode.SetValue(this->gameMode);
    text_mode.SetFont("Sansation");
    text_mode.SetColor(text_color); // Red text
    text_mode.SetBackground(Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f)); // Semi-transparent black background
    text_mode.SetPadding(10.0f, 5.0f); // 10px horizontal, 5px vertical padding
    text_mode.SetSize(20.0f);
    text_mode.SetStyle(Engine::Graphics::FontStyle::UPPERCASE);


    // In your text rendering code:
    text_status.SetValue("Hello World!");
    text_status.SetFont("Sansation");
    text_status.SetColor(text_color); // Red text
    text_status.SetBackground(Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f)); // Semi-transparent black background
    text_status.SetPadding(10.0f, 5.0f); // 10px horizontal, 5px vertical padding
    text_status.SetSize(20.0f);
    text_status.SetStyle(Engine::Graphics::FontStyle::UPPERCASE);
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateFadeIn(2.0f));
    text_status.GetAnimator().AddEffect(Engine::Graphics::Effects::CreateTypewriter(10.0f, true));
}

void Loading::OnShow() {
    std::cout << "[Loading] View shown" << std::endl;
}

void Loading::OnHide() {
    std::cout << "[Loading] View hidden" << std::endl;
}

void Loading::OnResize(int width, int height, int oldWidth, int oldHeight) {
    // Text positions are now handled proportionally in Render(), no need for manual resize handling
}

void Loading::OnUpdate(float deltaTime) {
    text_status.Update(deltaTime);
    text_map.SetValue(this->mapName);
    text_mode.SetValue(this->gameMode);
}

void Loading::OnUpdateProgress(const std::string& message, int actual, int total, float percentage) {

}

void Loading::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    float w = static_cast<float>(context.GetWidth());
    float h = static_cast<float>(context.GetHeight());

    // Set up 2D rendering for the entire view
    context.Begin2D(context.GetWidth(), context.GetHeight());

    /* Background */
    std::string file = GetBackgroundImage();
    if(!file.empty()) {
        static auto texture = context.LoadTexture("../Game/" + file);

        // Draw blurred background image
        context.DrawTextureBlurred(texture, 0.0f, 1.0f, w, h, 5.0f); // Blur radius

        // Draw horizontal scanlines overlay (CRT-style effect)
        context.DrawHorizontalLines(0.0f, 0.0f,
                                   w, h,
                                  3.0f,   // Line spacing (tight scanlines like CRT)
                                  4.0f,   // Line width (thin scanlines)
                                  Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.3f)); // Subtle but visible

        // Additional dark overlay to dim the entire image
        context.DrawRect(0.0f, 0.0f, w, h, Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.2f)); // Dark overlay

        // Add film grain for vintage/retro effect
        context.DrawFilmGrain(0.0f, 0.0f,
                             w, h,
                            0.15f,  // Grain intensity
                            123);   // Random seed
    }

    /* Text Rendering with Dynamic Resolution */
    // Use View's Dynamic Resolution system for consistent scaling
    text_map.Render(context, GetScaledX(20.0f), GetScaledY(30.0f));
    text_mode.Render(context, GetScaledX(20.0f), GetScaledY(82.0f));

    text_status.Render(context, Engine::Graphics::TextAlignment::CENTER); // Effects automatically applied

    context.End2D();
}
