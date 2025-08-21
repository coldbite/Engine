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

        // Dynamic Resolution System - Resolution Independent UI
        void SetReferenceResolution(float width, float height) { 
            referenceWidth = width; 
            referenceHeight = height; 
        }
        
        // Scaling factors
        float GetScaleX() const { return windowWidth / referenceWidth; }
        float GetScaleY() const { return windowHeight / referenceHeight; }
        
        // Uniform scaling (maintains aspect ratio)
        float GetUniformScale() const { return std::min(GetScaleX(), GetScaleY()); }
        
        // Coordinate transformation methods
        float GetScaledX(float baseX) const { return baseX * GetScaleX(); }
        float GetScaledY(float baseY) const { return baseY * GetScaleY(); }
        
        // Aspect-ratio preserving positioning (with letterboxing/pillarboxing)
        float GetCenteredX(float baseX) const { 
            float scale = GetUniformScale();
            float scaledWidth = referenceWidth * scale;
            float offsetX = (windowWidth - scaledWidth) * 0.5f;
            return offsetX + baseX * scale;
        }
        float GetCenteredY(float baseY) const { 
            float scale = GetUniformScale();
            float scaledHeight = referenceHeight * scale;
            float offsetY = (windowHeight - scaledHeight) * 0.5f;
            return offsetY + baseY * scale;
        }
        
        // Size scaling
        float GetScaledSize(float baseSize) const { return baseSize * GetUniformScale(); }
        
        // Reference resolution getters
        float GetReferenceWidth() const { return referenceWidth; }
        float GetReferenceHeight() const { return referenceHeight; }

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
        
        // Dynamic Resolution System
        float referenceWidth = 1280.0f;   // Default reference resolution
        float referenceHeight = 720.0f;

        // Helper for subclasses
        virtual void UpdateInternal(float /*deltaTime*/) {}
        virtual void RenderInternal() {}
    };

    using ViewPtr = std::shared_ptr<View>;
}
