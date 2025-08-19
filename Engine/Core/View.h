#pragma once

#include "Renderable.h"
#include "Event.h"
#include "../Graphics/IColor.h"
#include "../Graphics/RGB.h"
#include "../Graphics/RGBA.h"
#include "../Graphics/HEX.h"
#include <memory>
#include <algorithm>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }

    class View : public Renderable {
    public:
        View(const std::string& name = "View");
        virtual ~View() = default;

        bool IsActive() const { return isActive; }
        void SetActive(bool active);

        // Background color
        void SetBackground(const Graphics::IColor& color);
        const Graphics::IColor& GetBackground() const { return backgroundColor; }

        // Background Image
        void SetBackgroundImage(const std::string& name);
        const std::string& GetBackgroundImage() const { return backgroundImage; }

        // Window dimensions
        void SetWindowDimensions(int width, int height);
        int GetWindowWidth() const { return windowWidth; }
        int GetWindowHeight() const { return windowHeight; }

        // Responsive scaling helpers
        float GetScaleFactor() const { return std::min(windowWidth / 1024.0f, windowHeight / 768.0f); }
        float GetScaledX(float baseX) const { return baseX * (windowWidth / 1024.0f); }
        float GetScaledY(float baseY) const { return baseY * (windowHeight / 768.0f); }
        float GetScaledSize(float baseSize) const { return baseSize * GetScaleFactor(); }

        // View lifecycle methods
        virtual void OnShow() {}
        virtual void OnHide() {}
        virtual void OnUpdate(float /*deltaTime*/) {}
        virtual void OnResize(int width, int height, int oldWidth, int oldHeight) {}

        // Rendering with context - ONLY method for rendering
        virtual void Render(Graphics::IRenderingAPI& /*renderingAPI*/) {}

        // Override from Renderable
        void PrepareForRendering() override;

    protected:
        bool isActive;
        Graphics::RGBA backgroundColor;
        std::string backgroundImage;
        int windowWidth = 800;
        int windowHeight = 600;

        // Helper for subclasses
        virtual void UpdateInternal(float /*deltaTime*/) {}
        virtual void RenderInternal() {}
    };

    using ViewPtr = std::shared_ptr<View>;
}
