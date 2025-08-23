#include "Options.h"
#include "../../../../Engine/Graphics/IRenderingAPI.h"

Options::Options() {
    titleText.SetValue("OPTIONS");
    titleText.SetFont("Sansation");
    titleText.SetSize(28.0f);
    titleText.SetColor(new Engine::Graphics::RGBA(255, 255, 255, 255));
    titleText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
    titleText.SetPadding(10.0f, 10.0f);

    optionsText.SetValue("Game Settings:\n\nGraphics Quality: High\nResolution: 1280x720\nFullscreen: No\nV-Sync: Enabled\nSound Volume: 80%\nMusic Volume: 60%\n\nControls:\nMove: WASD\nJump: Space\nAttack: Left Click");
    optionsText.SetFont("Sansation");
    optionsText.SetSize(18.0f);
    optionsText.SetColor(new Engine::Graphics::RGBA(220, 220, 220, 255));
    optionsText.SetPadding(15.0f, 10.0f);
}

void Options::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    titleText.Render(context, x, y);
    optionsText.Render(context, x, y + 60.0f);
}

void Options::Update(float deltaTime) {
    titleText.Update(deltaTime);
    optionsText.Update(deltaTime);
}