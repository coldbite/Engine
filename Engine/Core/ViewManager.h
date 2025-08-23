#pragma once

#include "Engine.h"
#include "View.h"
#include "Event.h"
#include "NativeWindow.h"
#include <unordered_map>
#include <memory>

// Forward declarations
namespace Engine {
    class Engine;
    class Game;
}

// Forward declaration
class Overlay;

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }

    class ViewManager {
    public:
        ViewManager(Game* game = nullptr);
        ~ViewManager() = default;

        // View registration and management
        void RegisterView(const std::string& name, ViewPtr view);
        void UnregisterView(const std::string& name);

        // View control
        void ShowView(const std::string& name);
        void HideView(const std::string& name);
        void HideAllViews();
        
        // Overlay system
        void ShowOverlay(const std::string& name, ViewPtr overlay);
        void HideOverlay(const std::string& name);
        void HideAllOverlays();
        bool HasActiveOverlay() const;

        // Current view management
        const std::string& GetCurrentView() const { return currentView; }
        ViewPtr GetView(const std::string& name);
        ViewPtr GetCurrentViewPtr();

        // Update all active views
        void UpdateViews(float deltaTime);

        // Rendering
        void RenderViews(Graphics::IRenderingAPI& renderingAPI);
        void SetRenderTarget(std::shared_ptr<NativeWindow> window);
        void SetRenderingAPI(std::shared_ptr<Graphics::IRenderingAPI> api);

        // Event handling
        void OnViewChangeEvent(const ViewChangeEvent& event);
        
        // Window dimension updates
        void UpdateViewDimensions(int width, int height);
        
        // Engine access
        Engine& GetEngine() const;
        
        // Game access
        Game* GetGame() const { return gameInstance; }

    private:
        std::unordered_map<std::string, ViewPtr> views;
        std::string currentView;
        std::shared_ptr<NativeWindow> renderWindow;
        std::shared_ptr<Graphics::IRenderingAPI> renderingAPI;
        
        // Overlay system
        std::unordered_map<std::string, ViewPtr> overlays;
        
        // Transition state
        bool isTransitioning = false;
        float transitionProgress = 0.0f;
        Transition currentTransition = Transition::FADE;
        std::string transitionTargetView;
        std::string transitionSourceView;
        
        // Game instance
        Game* gameInstance = nullptr;

        void TransitionTo(const std::string& newView, Transition transition = Transition::FADE);
        void UpdateTransition(float deltaTime);
    };
}
