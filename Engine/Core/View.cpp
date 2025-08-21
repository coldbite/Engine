#include "View.h"
#include "../Graphics/IRenderingAPI.h"
#include <iostream>

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

    void View::PrepareForRendering() {
        if(isActive && IsVisible()) {
            RenderInternal();
            // Note: Render(IRenderingAPI&) is called by ViewManager, not here
        }
    }
}
