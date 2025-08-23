#include "View.h"
#include "../Graphics/IRenderingAPI.h"

namespace Engine {
    View::View(const std::string& name) :
        Renderable(name),
        isActive(false),
        backgroundColor(Graphics::RGBA(0.0f, 0.0f, 0.0f, 1.0f)) {
        /* Do Nothing */
    }

    void View::SetActive(bool active) {
        if(isActive == active) {
            return;
        }

        isActive = active;

        if(active) {
            SetVisible(true);
            OnShow();
        } else {
            SetVisible(false);
            // DON'T clear key bindings - the isActive check in callbacks handles this
            // ClearKeyBindings(); // This was causing all callbacks to be deleted!
            OnHide();
        }
    }

    void View::SetBackground(const Graphics::IColor& color) {
        backgroundColor = Graphics::RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
    }

    void View::SetBackgroundImage(const std::string& file) {
        backgroundImage = file;
    }

    void View::SetWindowDimensions(int width, int height) {
        if (windowWidth != width || windowHeight != height) {
            int oldWidth = windowWidth;
            int oldHeight = windowHeight;
            windowWidth = width;
            windowHeight = height;
            
            // Update viewport manager with new window dimensions
            viewportManager.SetWindowDimensions(width, height);
            
            OnResize(width, height, oldWidth, oldHeight);
        }
    }
    
    void View::ApplyViewport(Graphics::IRenderingAPI& renderingAPI) {
        ViewportInfo viewport = viewportManager.CalculateViewport();
        renderingAPI.SetViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    }
    

    void View::OnKey(Input::KEY key, std::function<void()> callback) {
        OnKey(key, Input::KeyAction::PRESS, callback);
    }

    void View::OnKey(Input::KEY key, Input::KeyAction action, std::function<void()> callback) {
        // Bind keys immediately - they will only trigger when view is active
        Input::GetInput().BindKey(key, action, [this, callback](Input::KEY, Input::KeyAction) {
            // Only execute callback if this view is currently active
            if (this->isActive) {
                callback();
            }
        });
        activeKeyBindings.push_back({key, action});
    }

    void View::ClearKeyBindings() {
        // Remove all key bindings for this view
        for (const auto& binding : activeKeyBindings) {
            Input::GetInput().UnbindKey(binding.first, binding.second);
        }
        activeKeyBindings.clear();
    }

    void View::PrepareForRendering() {
        if(isActive && IsVisible()) {
            RenderInternal();
            // Note: Render(IRenderingAPI&) is called by ViewManager, not here
        }
    }
}
