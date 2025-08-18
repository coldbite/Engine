#include "UIView.h"
#include <iostream>

namespace Engine {
    UIView::UIView(const std::string& name) 
        : Renderable(name), isActive(false) {
    }

    void UIView::SetActive(bool active) {
        if (isActive == active) {
            return;
        }

        isActive = active;
        
        if (active) {
            std::cout << "[UIView] Showing view: " << GetName() << std::endl;
            SetVisible(true);
            OnShow();
        } else {
            std::cout << "[UIView] Hiding view: " << GetName() << std::endl;
            SetVisible(false);
            OnHide();
        }
    }

    void UIView::PrepareForRendering() {
        if (isActive && IsVisible()) {
            RenderInternal();
            // Only render with the active API, not all APIs
            // For now, default to OpenGL until proper API detection is implemented
            RenderOpenGL();
        }
    }
}