#include "Score.h"
#include "../../../../Engine/Graphics/IRenderingAPI.h"

Score::Score() {
    titleText.SetValue("SCORE");
    titleText.SetFont("Sansation");
    titleText.SetSize(28.0f);
    titleText.SetColor(new Engine::Graphics::RGBA(255, 255, 255, 255));
    titleText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
    titleText.SetPadding(10.0f, 10.0f);

    scoreText.SetValue("Current Score: 1,450\nHigh Score: 2,870\nLevel: 5");
    scoreText.SetFont("Sansation");
    scoreText.SetSize(18.0f);
    scoreText.SetColor(new Engine::Graphics::RGBA(220, 220, 220, 255));
    scoreText.SetPadding(15.0f, 10.0f);
}

void Score::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    titleText.Render(context, x, y);
    scoreText.Render(context, x, y + 60.0f);
}

void Score::Update(float deltaTime) {
    titleText.Update(deltaTime);
    scoreText.Update(deltaTime);
}