#include "Playing.h"
#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Core/ViewManager.h"
#include <chrono>

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    playingText.SetValue("Playing game...!");
    playingText.SetFont("Sansation");
    playingText.SetSize(22.0f);
    playingText.SetColor(text_color);

    OnKey(Engine::Input::KEY::ESC, [this]() {
        if(!GetViewManager()) {
            return;
        }

        auto now            = std::chrono::steady_clock::now();
        float currentTime   = std::chrono::duration<float>(now.time_since_epoch()).count();

        if(currentTime - lastToggleTime < TOGGLE_DEBOUNCE_TIME) {
            return;
        }

        lastToggleTime = currentTime;

        if(overlayVisible) {
            GetViewManager()->HideOverlay("GameOverlay");
            overlayVisible = false;
        } else {
            if (!gameOverlay) {
                gameOverlay = std::make_shared<Overlay>();

                gameOverlay->SetBackground(new Engine::Graphics::RGBA(0, 0, 0, 70));
                gameOverlay->SetActive(true);
                gameOverlay->SetVisible(true);
            } else {
                gameOverlay->SetActive(true);
                gameOverlay->SetVisible(true);
            }

            GetViewManager()->ShowOverlay("GameOverlay", gameOverlay);
            overlayVisible = true;
        }
    });
}

void Playing::OnShow() {}
void Playing::OnHide() {}

void Playing::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void)width; (void)oldWidth;
    playingText.UpdateFontSizeForWindow(height, 22, oldHeight);
}

void Playing::OnUpdate(float deltaTime) { (void)deltaTime; }

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());
    context.Begin2D(context.GetWidth(), context.GetHeight());
    playingText.Render(context, Engine::Graphics::TextAlignment::CENTER);
    context.End2D();
}

