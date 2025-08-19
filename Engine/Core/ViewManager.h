#pragma once

#include "View.h"
#include "Event.h"
#include "NativeWindow.h"
#include <unordered_map>
#include <memory>

namespace Engine {
    class ViewManager {
    public:
        ViewManager();
        ~ViewManager() = default;

        // View registration and management
        void RegisterView(const std::string& name, ViewPtr view);
        void UnregisterView(const std::string& name);

        // View control
        void ShowView(const std::string& name);
        void HideView(const std::string& name);
        void HideAllViews();

        // Current view management
        const std::string& GetCurrentView() const { return currentView; }
        ViewPtr GetView(const std::string& name);
        ViewPtr GetCurrentViewPtr();

        // Update all active views
        void UpdateViews(float deltaTime);

        // Rendering
        void RenderViews();
        void SetRenderTarget(std::shared_ptr<NativeWindow> window);

        // Event handling
        void OnViewChangeEvent(const ViewChangeEvent& event);

    private:
        std::unordered_map<std::string, ViewPtr> views;
        std::string currentView;
        std::shared_ptr<NativeWindow> renderWindow;
        
        // Transition state
        bool isTransitioning = false;
        float transitionProgress = 0.0f;
        Transition currentTransition = Transition::FADE;
        std::string transitionTargetView;
        std::string transitionSourceView;

        void TransitionTo(const std::string& newView, Transition transition = Transition::FADE);
        void UpdateTransition(float deltaTime);
    };
}
