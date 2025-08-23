#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Core/ViewManager.h"
#include "../../../Engine/Core/Game.h"
#include "Playing.h"

Overlay::Overlay() : Engine::View("Overlay"), currentContent("") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    overlayText.SetValue("GAME MENU");
    overlayText.SetFont("Sansation");
    overlayText.SetColor(text_color);
    overlayText.SetBackground(new Engine::Graphics::RGBA(0, 0, 0, 30));
    overlayText.SetPadding(10.0f, 5.0f);
    overlayText.SetMargin(30.0f, 0.0f, 0.0f, 60.0f);
    overlayText.SetSize(40.0f);
    overlayText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);

    // Setup menu buttons
    gameMenu.AddButton("Resume", "resume");
    gameMenu.AddSpace(40);
    gameMenu.AddButton("Score", "score");
    gameMenu.AddButton("Assignments", "assignments");
    gameMenu.AddButton("Server Info", "serverinfo");
    gameMenu.AddButton("Options", "options");
    gameMenu.AddSpace(40);
    gameMenu.AddButton("Quit", "quit");
    gameMenu.AddSpace(40);

    // Setup menu callback
    gameMenu.OnKey([this](const std::string& key) {
        std::cout << "CLICKED BUTTON: " << key;

        if(key == "resume") {
            std::shared_ptr<View> v = GetViewManager()->GetView("Playing");
            Playing* playing        = dynamic_cast<Playing*>(v.get());
            playing->ToggleOverlay();
        } else if(key == "quit") {
            GetViewManager()->GetGame()->Exit();
        } else {
            ShowContent(key);
        }
    });
}

void Overlay::OnShow() {}
void Overlay::OnHide() {}

void Overlay::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void) width;
    (void) oldWidth;
    overlayText.UpdateFontSizeForWindow(height, 40, oldHeight);
}

void Overlay::OnUpdate(float deltaTime) {
    gameMenu.Update(deltaTime);
}

void Overlay::OnMouseMove(float x, float y) {
    gameMenu.OnMouseMove(x, y);
}

void Overlay::OnMouseDown(float x, float y) {
    gameMenu.OnMouseDown(x, y);
}

void Overlay::OnMouseUp(float x, float y) {
    gameMenu.OnMouseUp(x, y);
}


void Overlay::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Begin2D(context.GetWidth(), context.GetHeight());
    context.DrawRect(0, 0, context.GetWidth(), context.GetHeight(), GetBackground());

    // Render title
    overlayText.Render(context, Engine::Graphics::TextAlignment::TOP_LEFT);

    // Calculate menu area (left side - yellow area)
    float menuWidth = 300.0f;
    float menuHeight = static_cast<float>(context.GetHeight());

    // Render menu (yellow area with red buttons)
    gameMenu.Render(context, 60.0f, 0.0f, menuWidth, menuHeight);

    // Render content area (blue area) if content is selected
    if (!currentContent.empty()) {
        float contentX = 60.0f + menuWidth + 20.0f;
        float contentY = 100.0f;
        float contentWidth = context.GetWidth() - contentX - 60.0f;
        float contentHeight = context.GetHeight() - contentY - 60.0f;

        // Draw blue content background
        context.DrawRect(contentX, contentY, contentWidth, contentHeight,
                        new Engine::Graphics::RGBA(0, 0, 128, 60));

        // TODO: Render actual content views here based on currentContent
    }

    context.End2D();
}

void Overlay::ShowContent(const std::string& contentType) {
    currentContent = contentType;
    // Content will be rendered in the blue area
}

