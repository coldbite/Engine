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

        // Responsive scaling helpers - Smart scaling approach
        // Use different scaling for X and Y to fill the entire window
        float GetScaleX() const { return windowWidth / 1024.0f; }
        float GetScaleY() const { return windowHeight / 768.0f; }
        
        // Uniform scaling (for elements that must maintain aspect ratio)
        float GetUniformScale() const { return std::min(GetScaleX(), GetScaleY()); }
        
        // Full window scaling (for backgrounds, full-screen elements)
        float GetScaledX(float baseX) const { return baseX * GetScaleX(); }
        float GetScaledY(float baseY) const { return baseY * GetScaleY(); }
        
        // Centered positioning for aspect-ratio preserved elements
        float GetCenteredX(float baseX) const { 
            float scale = GetUniformScale();
            float scaledWidth = 1024.0f * scale;
            float offsetX = (windowWidth - scaledWidth) * 0.5f;
            return offsetX + baseX * scale;
        }
        float GetCenteredY(float baseY) const { 
            float scale = GetUniformScale();
            float scaledHeight = 768.0f * scale;
            float offsetY = (windowHeight - scaledHeight) * 0.5f;
            return offsetY + baseY * scale;
        }
        
        // Size scaling
        float GetScaledSize(float baseSize) const { return baseSize * GetUniformScale(); }

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
