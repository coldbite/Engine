#include "Assignments.h"
#include "../../../../Engine/Graphics/IRenderingAPI.h"

Assignments::Assignments() {
    titleText.SetValue("ASSIGNMENTS");
    titleText.SetFont("Sansation");
    titleText.SetSize(28.0f);
    titleText.SetColor(new Engine::Graphics::RGBA(255, 255, 255, 255));
    titleText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
    titleText.SetPadding(10.0f, 10.0f);

    assignmentText.SetValue("Active Assignments:\n\n• Collect 5 power-ups\n• Reach level 6\n• Defeat the boss\n• Complete mission in under 10 minutes");
    assignmentText.SetFont("Sansation");
    assignmentText.SetSize(18.0f);
    assignmentText.SetColor(new Engine::Graphics::RGBA(220, 220, 220, 255));
    assignmentText.SetPadding(15.0f, 10.0f);
}

void Assignments::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    titleText.Render(context, x, y);
    assignmentText.Render(context, x, y + 60.0f);
}

void Assignments::Update(float deltaTime) {
    titleText.Update(deltaTime);
    assignmentText.Update(deltaTime);
}