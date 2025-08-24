#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Core/ViewManager.h"
#include "../../../Engine/Core/Game.h"
#include "Playing.h"

Overlay::Overlay() : Engine::View("Overlay"), currentContent(""), contentScrollY(0.0f) {
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

    // Initialize content instances
    scoreContent = std::make_unique<Score>();
    assignmentsContent = std::make_unique<Assignments>();
    optionsContent = std::make_unique<Options>();
    serverInfoContent = std::make_unique<ServerInfo>();

    // Setup menu callback
    gameMenu.OnKey([this](const std::string& key) {
        std::cout << "CLICKED BUTTON: " << key;

        if(key == "resume" || key == "quit") {
            std::shared_ptr<View> v = GetViewManager()->GetView("Playing");
            Playing* playing        = dynamic_cast<Playing*>(v.get());
            playing->ToggleOverlay();

            if(key == "quit") {
                GetViewManager()->GetGame()->Exit();
            }
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
    
    // Update current content
    if (!currentContent.empty()) {
        if (currentContent == "score" && scoreContent) {
            scoreContent->Update(deltaTime);
        } else if (currentContent == "assignments" && assignmentsContent) {
            assignmentsContent->Update(deltaTime);
        } else if (currentContent == "options" && optionsContent) {
            optionsContent->Update(deltaTime);
        } else if (currentContent == "serverinfo" && serverInfoContent) {
            serverInfoContent->Update(deltaTime);
        }
    }
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

void Overlay::OnMouseScroll(float x, float y) {
    // Only handle scrolling if we're in the content area and have content
    if (!currentContent.empty()) {
        float menuWidth = 300.0f;
        float contentX = 60.0f + menuWidth + 20.0f;
        float contentY = 100.0f;
        
        // Check if mouse is in content area
        if (x >= contentX) {
            // Scroll the content
            contentScrollY += y * 30.0f; // Scroll speed multiplier
            
            // Clamp scroll to prevent scrolling too far up
            if (contentScrollY > 0) {
                contentScrollY = 0;
            }
        }
    }
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

        // Set up clipping area for scrollable content
        // Apply scroll offset to content rendering position
        float scrolledContentY = contentY + contentScrollY + 20.0f; // Add padding
        float scrolledContentX = contentX + 20.0f; // Add padding
        float scrolledContentWidth = contentWidth - 40.0f; // Account for padding
        float scrolledContentHeight = contentHeight - 40.0f; // Account for padding

        // Render the appropriate content based on currentContent
        if (currentContent == "score" && scoreContent) {
            scoreContent->Render(context, scrolledContentX, scrolledContentY, scrolledContentWidth, scrolledContentHeight);
        } else if (currentContent == "assignments" && assignmentsContent) {
            assignmentsContent->Render(context, scrolledContentX, scrolledContentY, scrolledContentWidth, scrolledContentHeight);
        } else if (currentContent == "options" && optionsContent) {
            optionsContent->Render(context, scrolledContentX, scrolledContentY, scrolledContentWidth, scrolledContentHeight);
        } else if (currentContent == "serverinfo" && serverInfoContent) {
            serverInfoContent->Render(context, scrolledContentX, scrolledContentY, scrolledContentWidth, scrolledContentHeight);
        }

        // Draw scrollbar if needed
        // Simple scrollbar on the right side of the content area
        float scrollbarWidth = 10.0f;
        float scrollbarX = contentX + contentWidth - scrollbarWidth - 5.0f;
        float scrollbarY = contentY + 5.0f;
        float scrollbarHeight = contentHeight - 10.0f;
        
        // Draw scrollbar background
        context.DrawRect(scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight,
                        new Engine::Graphics::RGBA(64, 64, 64, 100));
        
        // Draw scrollbar thumb (simplified - always show for now)
        if (contentScrollY < 0) {
            float thumbHeight = scrollbarHeight * 0.3f; // Approximate thumb size
            float thumbY = scrollbarY + (-contentScrollY / 500.0f) * (scrollbarHeight - thumbHeight);
            context.DrawRect(scrollbarX + 1, thumbY, scrollbarWidth - 2, thumbHeight,
                            new Engine::Graphics::RGBA(128, 128, 128, 180));
        }
    }

    context.End2D();
}

void Overlay::ShowContent(const std::string& contentType) {
    currentContent = contentType;
    contentScrollY = 0.0f; // Reset scroll position when switching content
}

