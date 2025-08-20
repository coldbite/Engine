#include "Loading.h"
#include "../../../Engine/Graphics/OpenGL/OpenGL.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#ifdef _WIN32
    #include <windows.h>
    #include <gl/gl.h>
#endif

Loading::Loading()
    : Engine::View("Loading")
    , currentMessage("Initializing...")
    , currentStep(0)
    , totalSteps(0)
    , progressPercentage(0.0f)
    , startTime(std::chrono::steady_clock::now()) {

    SetBackground(color_background);
    title.LoadFont("../Game/Assets/Fonts/Sansation-Bold.ttf", 40);
    text1.LoadFont("../Game/Assets/Fonts/Sansation-Regular.ttf", 18);
}

void Loading::OnShow() {
    /* Do Nothing */
}

void Loading::OnHide() {
    /* Do Nothing */
}

void Loading::OnResize(int width, int height, int oldWidth, int oldHeight) {
    title.UpdateFontSizeForWindow(height, 40, oldHeight);
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
}

void Loading::Render(Engine::Graphics::IRenderingAPI& context) {
    // Clear screen with static background color
    context.Clear(GetBackground());

    // Get actual window dimensions for the entire function
    int actualWidth = context.GetWidth();
    int actualHeight = context.GetHeight();

    // Set up 2D rendering with actual context dimensions
    context.Begin2D(actualWidth, actualHeight);

    // Rendering Background
    std::string file = GetBackgroundImage();
    if(!file.empty()) {
        static auto texture = context.LoadTexture("../Game/" + file);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(actualWidth, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(actualWidth, actualHeight);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, actualHeight);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    // Header
    title.RenderText(context, mapName, 40.0f, 70.0f, Engine::Graphics::RGBA(1.0f, 1.0f, 1.0f, 0.8f));

    // Render animated loading text (center of screen)
    RenderAnimatedText(context, actualWidth, actualHeight);

    context.End2D();
}

// Animation helper methods implementation
float Loading::GetAnimationTime() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    float totalSeconds = elapsed.count() / 1000.0f;

    // Loop animation every animationCycleDuration seconds
    return fmod(totalSeconds, animationCycleDuration);
}

float Loading::GetTitleAlpha(float animTime) const {
    // Title animation: 0% opacity until 48%, then fade in at 52%, stay until 70%, then fade out
    float progress = animTime / animationCycleDuration; // 0.0 to 1.0

    if (progress < 0.48f) return 0.0f;
    if (progress < 0.52f) return (progress - 0.48f) / 0.04f; // Fade in over 4%
    if (progress < 0.70f) return 1.0f; // Full opacity
    if (progress < 1.0f) return 1.0f - ((progress - 0.70f) / 0.30f); // Fade out over 30%
    return 0.0f;
}

float Loading::GetTextAlpha(float animTime) const {
    // Text animation: Same as title but delayed by 1.7 seconds (50% of cycle)
    float delayedTime = fmod(animTime + animationCycleDuration * 0.5f, animationCycleDuration);
    return GetTitleAlpha(delayedTime);
}

float Loading::GetTitleX(float animTime) const {
    // X position animation: smooth sliding from right to center during visible period
    float progress = animTime / animationCycleDuration;
    float baseX = windowWidth * 0.5f; // Center of window

    if (progress < 0.48f) return baseX + 130.0f;
    if (progress < 0.70f) {
        // Smooth easing during entire visible period (0.48 to 0.70)
        float t = (progress - 0.48f) / 0.22f; // 0.0 to 1.0 over 22% of cycle
        // Apply easing function for smooth motion
        float eased = 1.0f - pow(1.0f - t, 3.0f); // Ease-out cubic
        return baseX + 130.0f - (100.0f * eased); // Slide from +130 to +30
    }
    return baseX + 30.0f;
}

bool Loading::GetCursorVisible(float animTime) const {
    // Blink animation: complex pattern with position changes
    float progress = animTime / animationCycleDuration;

    // Simplified blinking pattern - alternating visibility every 0.05 seconds
    float blinkTime = fmod(animTime, 0.1f);
    return blinkTime < 0.05f;
}

Engine::Graphics::RGBA Loading::GetAnimatedTextShadow(float animTime) const {
    // Text shadow color animation: #306f99 -> #19470f -> #734a10
    float progress = animTime / animationCycleDuration;

    if (progress < 0.5f) {
        // From blue (#306f99) to green (#19470f)
        float t = progress * 2.0f; // 0.0 to 1.0
        float r = 0x30/255.0f * (1-t) + 0x19/255.0f * t;
        float g = 0x6f/255.0f * (1-t) + 0x47/255.0f * t;
        float b = 0x99/255.0f * (1-t) + 0x0f/255.0f * t;
        return Engine::Graphics::RGBA(r, g, b, 0.8f); // Semi-transparent for glow effect
    } else if (progress < 0.9f) {
        // From green (#19470f) to brown (#734a10)
        float t = (progress - 0.5f) / 0.4f; // 0.0 to 1.0
        float r = 0x19/255.0f * (1-t) + 0x73/255.0f * t;
        float g = 0x47/255.0f * (1-t) + 0x4a/255.0f * t;
        float b = 0x0f/255.0f * (1-t) + 0x10/255.0f * t;
        return Engine::Graphics::RGBA(r, g, b, 0.8f); // Semi-transparent for glow effect
    } else {
        // Stay brown for last 10%
        return Engine::Graphics::RGBA(0x73/255.0f, 0x4a/255.0f, 0x10/255.0f, 0.8f);
    }
}

void Loading::RenderAnimatedText(Engine::Graphics::IRenderingAPI& renderingAPI, int windowWidth, int windowHeight) {
    float animTime = GetAnimationTime();

    // Get animation values
    float titleAlpha = GetTitleAlpha(animTime);
    float textAlpha = GetTextAlpha(animTime);
    float titleX = GetTitleX(animTime);
    bool cursorVisible = GetCursorVisible(animTime);
    auto shadowColor = GetAnimatedTextShadow(animTime);

    // Center container in actual window
    float centerX = windowWidth * 0.5f;
    float centerY = windowHeight * 0.5f;

    // Container dimensions with uniform scaling to maintain aspect ratio
    float baseBoxWidth = 400.0f;
    float baseBoxHeight = 120.0f;
    // Calculate scaling based on actual window dimensions
    float scaleX = windowWidth / 1024.0f;
    float scaleY = windowHeight / 768.0f;
    float uniformScale = std::min(scaleX, scaleY);
    float boxWidth = baseBoxWidth * uniformScale;
    float boxHeight = baseBoxHeight * uniformScale;
    float boxX = centerX - boxWidth * 0.5f;
    float boxY = centerY - boxHeight * 0.5f;

    // Render background box with animated border (like the GIF)
    renderingAPI.DrawRect(boxX, boxY, boxWidth, boxHeight, ::Engine::Graphics::RGBA(0.05f, 0.05f, 0.1f, 0.95f)); // Dark background

    // Animated border with shadow color
    renderingAPI.DrawRect(boxX - 2, boxY - 2, boxWidth + 4, 2, shadowColor); // Top
    renderingAPI.DrawRect(boxX - 2, boxY + boxHeight, boxWidth + 4, 2, shadowColor); // Bottom
    renderingAPI.DrawRect(boxX - 2, boxY, 2, boxHeight, shadowColor); // Left
    renderingAPI.DrawRect(boxX + boxWidth, boxY, 2, boxHeight, shadowColor); // Right

    // Text positioning with uniform scaling
    float textAreaX = boxX + boxWidth - (30.0f * uniformScale); // Right side with scaled margin
    float textScale = uniformScale;

    // Glow offset calculations for consistent use
    float glowOffset1 = uniformScale;
    float glowOffset2 = uniformScale * 2.0f;
    float glowOffset3 = uniformScale * 3.0f;

    // Render "loading" title with smooth glow effect
    if (titleAlpha > 0.0f) {
        float actualX = titleX - (120.0f * uniformScale); // Use animated X position with scaling

        // Create multiple glow layers for softer effect
        Engine::Graphics::RGBA glowColor1(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * titleAlpha * 0.3f);
        Engine::Graphics::RGBA glowColor2(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * titleAlpha * 0.2f);
        Engine::Graphics::RGBA glowColor3(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * titleAlpha * 0.1f);

        // Multiple glow passes for smooth shadow with responsive scaling

        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset1, centerY - 9, glowColor1);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset1, centerY - 11, glowColor1);
        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset1, centerY - 11, glowColor1);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset1, centerY - 9, glowColor1);

        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset2, centerY - 8, glowColor2);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset2, centerY - 12, glowColor2);
        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset2, centerY - 12, glowColor2);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset2, centerY - 8, glowColor2);

        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset3, centerY - 7, glowColor3);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset3, centerY - 13, glowColor3);
        text1.RenderText(renderingAPI, "LOADING", actualX + glowOffset3, centerY - 13, glowColor3);
        text1.RenderText(renderingAPI, "LOADING", actualX - glowOffset3, centerY - 7, glowColor3);

        // Main text on top
        Engine::Graphics::RGBA titleColor(1.0f, 1.0f, 1.0f, titleAlpha);
        text1.RenderText(renderingAPI, "LOADING", actualX, centerY - 10, titleColor);
    }

    // Render map name text with smooth glow effect (delayed)
    if (textAlpha > 0.0f) {
        float actualX = titleX - (120.0f * uniformScale); // Use same animated X position with scaling

        // Create multiple glow layers for softer effect
        Engine::Graphics::RGBA glowColor1(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * textAlpha * 0.3f);
        Engine::Graphics::RGBA glowColor2(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * textAlpha * 0.2f);
        Engine::Graphics::RGBA glowColor3(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * textAlpha * 0.1f);

        text1.RenderText(renderingAPI, mapName, actualX + glowOffset1, centerY + 16, glowColor1);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset1, centerY + 14, glowColor1);
        text1.RenderText(renderingAPI, mapName, actualX + glowOffset1, centerY + 14, glowColor1);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset1, centerY + 16, glowColor1);

        text1.RenderText(renderingAPI, mapName, actualX + glowOffset2, centerY + 17, glowColor2);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset2, centerY + 13, glowColor2);
        text1.RenderText(renderingAPI, mapName, actualX + glowOffset2, centerY + 13, glowColor2);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset2, centerY + 17, glowColor2);

        text1.RenderText(renderingAPI, mapName, actualX + glowOffset3, centerY + 18, glowColor3);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset3, centerY + 12, glowColor3);
        text1.RenderText(renderingAPI, mapName, actualX + glowOffset3, centerY + 12, glowColor3);
        text1.RenderText(renderingAPI, mapName, actualX - glowOffset3, centerY + 18, glowColor3);

        // Main text on top
        Engine::Graphics::RGBA textColor(1.0f, 1.0f, 1.0f, textAlpha);
        text1.RenderText(renderingAPI, mapName, actualX, centerY + 15, textColor);
    }

    // Render cursor (thin vertical line like in GIF) with responsive scaling
    if (cursorVisible && (titleAlpha > 0.0f || textAlpha > 0.0f)) {
        float cursorX = textAreaX - (20.0f * uniformScale);
        float cursorY = (titleAlpha > textAlpha) ? centerY - 10 : centerY + 15;
        float cursorWidth = uniformScale * 2.0f;
        float cursorHeight = 18.0f * uniformScale;

        // Cursor glow effect
        Engine::Graphics::RGBA cursorGlow(shadowColor.GetRed(), shadowColor.GetGreen(), shadowColor.GetBlue(), shadowColor.GetAlpha() * 0.5f);
        renderingAPI.DrawRect(cursorX + glowOffset1, cursorY + glowOffset1, cursorWidth, cursorHeight, cursorGlow);
        renderingAPI.DrawRect(cursorX - glowOffset1, cursorY - glowOffset1, cursorWidth, cursorHeight, cursorGlow);

        // Main cursor
        renderingAPI.DrawRect(cursorX, cursorY, cursorWidth, cursorHeight, ::Engine::Graphics::RGBA(1.0f, 1.0f, 1.0f, 1.0f));
    }
}
