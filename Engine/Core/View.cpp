#include "View.h"
#include <iostream>

namespace Engine {
    View::View(const std::string& name) : Renderable(name), isActive(false) {
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

    void View::PrepareForRendering() {
        if(isActive && IsVisible()) {
            RenderInternal();
            // Note: Render(IRenderingAPI&) is called by ViewManager, not here
        }
    }
}
