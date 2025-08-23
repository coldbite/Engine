#include "Playing.h"
#include "Overlay.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../Engine/Core/ViewManager.h"
#include <chrono>

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    SetReferenceResolution(1280.0f, 720.0f);

    OnKey(Engine::Input::KEY::ESC, [this]() {
        ToggleOverlay();
    });
    
    // Mouse event bindings
    OnMouseMove([this](float x, float y, float deltaX, float deltaY) {
        (void)deltaX; (void)deltaY;
        if (gameOverlay && overlayVisible) {
            gameOverlay->OnMouseMove(x, y);
        }
    });
    
    OnMouseButton(Engine::Input::MouseButton::LEFT, [this](float x, float y) {
        if (gameOverlay && overlayVisible) {
            gameOverlay->OnMouseDown(x, y);
            gameOverlay->OnMouseUp(x, y);
        }
    });
}

void Playing::ToggleOverlay() {
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

            gameOverlay->SetBackground(new Engine::Graphics::RGBA(0, 0, 0, 65));
            gameOverlay->SetActive(true);
            gameOverlay->SetVisible(true);
        } else {
            gameOverlay->SetActive(true);
            gameOverlay->SetVisible(true);
        }

        GetViewManager()->ShowOverlay("GameOverlay", gameOverlay);
        overlayVisible = true;
    }
}

void Playing::OnShow() {}
void Playing::OnHide() {}

void Playing::OnResize(int width, int height, int oldWidth, int oldHeight) {
    (void)width; (void)oldWidth;
    (void) height; (void) oldHeight;
}

void Playing::OnUpdate(float deltaTime) { 
    (void)deltaTime; 
}

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());
}

