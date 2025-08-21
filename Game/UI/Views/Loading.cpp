#include "Loading.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Loading::Loading() : Engine::View("Loading") {
    SetBackground(color_background);

    text_map.LoadFont("../Game/Assets/Fonts/Sansation-Regular.ttf", 40);
    text_mode.LoadFont("../Game/Assets/Fonts/Sansation-Regular.ttf", 20);
}

void Loading::OnShow() {
    std::cout << "[Loading] View shown" << std::endl;
}

void Loading::OnHide() {
    std::cout << "[Loading] View hidden" << std::endl;
}

void Loading::OnResize(int width, int height, int oldWidth, int oldHeight) {
    // Resize text to maintain relative size
    text_map.UpdateFontSizeForWindow(height, 40, oldHeight);
}

void Loading::OnUpdate(float deltaTime) {
    // Currently no internal updates needed for Loading view
    // UpdateInternal(deltaTime) would be called here if we had animations or game logic
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

    /* Map Name */
    std::string mode = this->gameMode;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper);

    TextBox(context, text_map, this->mapName, 20, 30);
    TextBox(context, text_mode, mode, 20, 90);

    context.End2D();
}

void Loading::TextBox(Engine::Graphics::IRenderingAPI& context, Engine::Graphics::Text& font, const std::string& text, float x, float y) {
    float padding_x = 10.0f;
    float padding_y = 10.0f;
    float textWidth = font.GetTextWidth(text);
    float textHeight = font.GetActualTextHeight(text, 1.0f);
    float boxWidth = textWidth + (padding_x * 2);
    float boxHeight = textHeight + (padding_y * 2);

    context.DrawRect(x, y, boxWidth, boxHeight, Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, 0.4f));
    font.RenderText(context, text, x + padding_x, y + padding_y, text_color);
}

