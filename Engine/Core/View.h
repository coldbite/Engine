#pragma once

#include "Renderable.h"
#include "Event.h"
#include "ViewportManager.h"
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

        // Viewport and scaling system
        ViewportManager& GetViewportManager() { return viewportManager; }
        const ViewportManager& GetViewportManager() const { return viewportManager; }
        
        // Legacy methods for backward compatibility - now use ViewportManager
        void SetReferenceResolution(float width, float height) { 
            viewportManager.SetReferenceResolution((int)width, (int)height);
        }
        
        // Scaling factors
        float GetScaleX() const { return viewportManager.GetScaleX(); }
        float GetScaleY() const { return viewportManager.GetScaleY(); }
        
        // Uniform scaling (maintains aspect ratio)
        float GetUniformScale() const { return viewportManager.GetUniformScale(); }
        
        // Coordinate transformation methods
        float GetScaledX(float baseX) const { return viewportManager.TransformX(baseX); }
        float GetScaledY(float baseY) const { return viewportManager.TransformY(baseY); }
        
        // Aspect-ratio preserving positioning (with letterboxing/pillarboxing)
        float GetCenteredX(float baseX) const { return viewportManager.TransformX(baseX); }
        float GetCenteredY(float baseY) const { return viewportManager.TransformY(baseY); }
        
        // Size scaling
        float GetScaledSize(float baseSize) const { 
            ViewportInfo viewport = viewportManager.CalculateViewport();
            return baseSize * std::min(viewport.scaleX, viewport.scaleY); 
        }
        
        // Reference resolution getters
        float GetReferenceWidth() const { return (float)viewportManager.GetReferenceWidth(); }
        float GetReferenceHeight() const { return (float)viewportManager.GetReferenceHeight(); }
        
        // Apply viewport to rendering API
        void ApplyViewport(Graphics::IRenderingAPI& renderingAPI);

        // View lifecycle methods
        virtual void OnShow() {}
        virtual void OnHide() {}
        virtual void OnUpdate(float /*deltaTime*/) {}
        virtual void OnResize(int /*width*/, int /*height*/, int /*oldWidth*/, int /*oldHeight*/) {}

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
        
        // Viewport and scaling system
        ViewportManager viewportManager;

        // Helper for subclasses
        virtual void UpdateInternal(float /*deltaTime*/) {}
        virtual void RenderInternal() {}
    };

    using ViewPtr = std::shared_ptr<View>;
}
