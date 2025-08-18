#include "ViewManager.h"
#include <iostream>
#include <algorithm>

namespace Engine {
    ViewManager::ViewManager() : currentView("") {
        std::cout << "[ViewManager] Initialized" << std::endl;
    }

    void ViewManager::RegisterView(const std::string& name, ViewPtr view) {
        if (!view) {
            std::cout << "[ViewManager] Warning: Attempted to register null view" << std::endl;
            return;
        }

        views[name] = view;
        std::cout << "[ViewManager] Registered view: " << view->GetName()
                  << " with name '" << name << "'" << std::endl;
    }

    void ViewManager::UnregisterView(const std::string& name) {
        auto it = views.find(name);
        if (it != views.end()) {
            if (currentView == name) {
                it->second->SetActive(false);
                currentView = "";
            }
            std::cout << "[ViewManager] Unregistered view: " << name << std::endl;
            views.erase(it);
        }
    }

    void ViewManager::ShowView(const std::string& name) {
        auto it = views.find(name);
        if (it == views.end()) {
            std::cout << "[ViewManager] Warning: View '" << name
                      << "' not registered" << std::endl;
            return;
        }

        TransitionTo(name);
    }

    void ViewManager::HideView(const std::string& name) {
        auto it = views.find(name);
        if (it != views.end()) {
            it->second->SetActive(false);
            if (currentView == name) {
                currentView = "";
            }
        }
    }

    void ViewManager::HideAllViews() {
        for (auto& [name, view] : views) {
            if (view) {
                view->SetActive(false);
            }
        }
        currentView = "";
        std::cout << "[ViewManager] All views hidden" << std::endl;
    }

    ViewPtr ViewManager::GetView(const std::string& name) {
        auto it = views.find(name);
        return (it != views.end()) ? it->second : nullptr;
    }

    ViewPtr ViewManager::GetCurrentViewPtr() {
        return GetView(currentView);
    }

    void ViewManager::UpdateViews(float deltaTime) {
        for (auto& [name, view] : views) {
            if (view && view->IsActive()) {
                view->OnUpdate(deltaTime);
            }
        }
    }

    void ViewManager::OnViewChangeEvent(const ViewChangeEvent& event) {
        ShowView(event.GetTargetView());
    }

    void ViewManager::TransitionTo(const std::string& newView) {
        if (currentView == newView) {
            return;
        }

        // Hide current view
        if (!currentView.empty()) {
            auto currentViewPtr = GetView(currentView);
            if (currentViewPtr) {
                currentViewPtr->SetActive(false);
            }
        }

        // Show new view
        auto newViewPtr = GetView(newView);
        if (newViewPtr) {
            newViewPtr->SetActive(true);
            currentView = newView;
            std::cout << "[ViewManager] Transitioned to view '" << newView << "'" << std::endl;
        } else {
            currentView = "";
            std::cout << "[ViewManager] Warning: Could not transition to view '"
                      << newView << "'" << std::endl;
        }
    }

    void ViewManager::RenderViews() {
        if (!renderWindow || !renderWindow->IsValid()) {
            return;
        }

        // Make the window's rendering context current
        renderWindow->MakeContextCurrent();

        // Clear the screen (basic rendering setup)
        // This would normally be done by the rendering API (OpenGL/DirectX/Vulkan)
        // For now, just ensure the context is ready

        // Render all active views
        for (auto& [name, view] : views) {
            if (view && view->IsActive() && view->IsVisible()) {
                view->PrepareForRendering();
            }
        }

        // Present the frame
        renderWindow->SwapBuffers();
    }

    void ViewManager::SetRenderTarget(std::shared_ptr<NativeWindow> window) {
        renderWindow = window;
        std::cout << "[ViewManager] Render target set to window: "
                  << (window ? window->GetTitle() : "nullptr") << std::endl;
    }
}
