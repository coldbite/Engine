#include "View.h"
#include <iostream>

namespace Engine {
    View::View(const std::string& name)
        : Renderable(name), isActive(false) {
    }

    void View::SetActive(bool active) {
        if (isActive == active) {
            return;
        }

        isActive = active;

        if (active) {
            std::cout << "[View] Showing view: " << GetName() << std::endl;
            SetVisible(true);
            OnShow();
        } else {
            std::cout << "[View] Hiding view: " << GetName() << std::endl;
            SetVisible(false);
            OnHide();
        }
    }

    void View::PrepareForRendering() {
        if (isActive && IsVisible()) {
            RenderInternal();
            Render();
        }
    }
}
