#pragma once

#include "UIView.h"
#include "Event.h"
#include <unordered_map>
#include <memory>

namespace Engine {
    class ViewManager {
    public:
        ViewManager();
        ~ViewManager() = default;

        // View registration and management
        void RegisterView(const std::string& name, UIViewPtr view);
        void UnregisterView(const std::string& name);
        
        // View control
        void ShowView(const std::string& name);
        void HideView(const std::string& name);
        void HideAllViews();
        
        // Current view management
        const std::string& GetCurrentView() const { return currentView; }
        UIViewPtr GetView(const std::string& name);
        UIViewPtr GetCurrentViewPtr();
        
        // Update all active views
        void UpdateViews(float deltaTime);
        
        // Event handling
        void OnViewChangeEvent(const ViewChangeEvent& event);

    private:
        std::unordered_map<std::string, UIViewPtr> views;
        std::string currentView;
        
        void TransitionTo(const std::string& newView);
    };
}