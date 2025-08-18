#pragma once

#include "Renderable.h"
#include "Event.h"
#include <memory>

namespace Engine {
    class View : public Renderable {
    public:
        View(const std::string& name = "View");
        virtual ~View() = default;

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

        virtual void Render() {}
    };

    using ViewPtr = std::shared_ptr<View>;
}
