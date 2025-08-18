#pragma once

#include "Renderable.h"
#include "Event.h"
#include <memory>

namespace Engine {
    class UIView : public Renderable {
    public:
        UIView(const std::string& name = "UIView");
        virtual ~UIView() = default;
        
        bool IsActive() const { return isActive; }
        void SetActive(bool active);

        // View lifecycle methods
        virtual void OnShow() {}
        virtual void OnHide() {}
        virtual void OnUpdate(float deltaTime) {}
        
        // Override from Renderable
        void PrepareForRendering() override;

    protected:
        bool isActive;
        
        // Helper for subclasses
        virtual void UpdateInternal(float deltaTime) {}
        virtual void RenderInternal() {}
        
        // Rendering methods for different APIs
        virtual void RenderOpenGL() {}
        virtual void RenderDirectX() {}
        virtual void RenderVulkan() {}
    };

    using UIViewPtr = std::shared_ptr<UIView>;
}