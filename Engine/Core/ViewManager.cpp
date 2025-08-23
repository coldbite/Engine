#include "ViewManager.h"
#include "Engine.h"
#include "../Graphics/OpenGL/OpenGL.h"
#include "../Graphics/IRenderingAPI.h"
#include "../../Game/UI/Views/Overlay.h"
#include <iostream>
#include <algorithm>

namespace Engine {
    ViewManager::ViewManager(Game* game) : currentView(""), gameInstance(game) {
        /* Do Nothing */
    }

    void ViewManager::RegisterView(const std::string& name, ViewPtr view) {
        if(!view) {
            //std::cout << "[ViewManager] Warning: Attempted to register null view" << std::endl;
            return;
        }

        views[name] = view;

        // Set ViewManager reference
        view->SetViewManager(this);

        // Initialize view with current window dimensions if available
        if(renderWindow) {
            const auto& props = renderWindow->GetProperties();
            view->SetWindowDimensions(props.width, props.height);
        }

        /*std::cout << "[ViewManager] Registered view: " << view->GetName()
                  << " with name '" << name << "' (Active: " << view->IsActive()
                  << ", Visible: " << view->IsVisible() << ")" << std::endl;*/
    }

    void ViewManager::UnregisterView(const std::string& name) {
        auto it = views.find(name);

        if(it != views.end()) {
            if(currentView == name) {
                it->second->SetActive(false);
                currentView = "";
            }

           // std::cout << "[ViewManager] Unregistered view: " << name << std::endl;
            views.erase(it);
        }
    }

    void ViewManager::ShowView(const std::string& name) {
        auto it = views.find(name);

        if(it == views.end()) {
           //std::cout << "[ViewManager] Warning: View '" << name << "' not registered" << std::endl;
            return;
        }

        TransitionTo(name);
    }

    void ViewManager::HideView(const std::string& name) {
        auto it = views.find(name);

        if(it != views.end()) {
            it->second->SetActive(false);

            if(currentView == name) {
                currentView = "";
            }
        }
    }

    void ViewManager::HideAllViews() {
        for(auto& [name, view] : views) {
            if(view) {
                view->SetActive(false);
            }
        }

        currentView = "";
    }

    ViewPtr ViewManager::GetView(const std::string& name) {
        auto it = views.find(name);
        return (it != views.end()) ? it->second : nullptr;
    }

    ViewPtr ViewManager::GetCurrentViewPtr() {
        return GetView(currentView);
    }

    bool ViewManager::HasView(const std::string& name) const {
        return views.find(name) != views.end();
    }

    void ViewManager::UpdateViews(float deltaTime) {
        // Update transition state
        if(isTransitioning) {
            // std::cout << "[ViewManager] UpdateViews: Transitioning, progress=" << transitionProgress << std::endl;
        }
        UpdateTransition(deltaTime);

        for(auto& [name, view] : views) {
            if(view && view->IsActive()) {
                view->OnUpdate(deltaTime);
            }
        }

        // Update overlays
        for(auto& [name, overlay] : overlays) {
            if(overlay && overlay->IsActive()) {
                overlay->OnUpdate(deltaTime);
            }
        }
    }

    void ViewManager::OnViewChangeEvent(const ViewChangeEvent& event) {
       //  std::cout << "[ViewManager] OnViewChangeEvent: target='" << event.GetTargetView() << "', transition=" << static_cast<int>(event.GetTransition()) << std::endl;
        TransitionTo(event.GetTargetView(), event.GetTransition());
    }

    void ViewManager::TransitionTo(const std::string& newView, Transition transition) {
        if(currentView == newView && !isTransitioning) {
            //std::cout << "[ViewManager] Already showing view: " << newView << std::endl;
            return;
        }

        // Start transition
        isTransitioning = true;
        transitionProgress = 0.0f;
        currentTransition = transition;
        transitionSourceView = currentView;
        transitionTargetView = newView;

        // std::cout << "[ViewManager] Starting transition from '" << transitionSourceView << "' to '" << transitionTargetView << "'" << std::endl;

        // Keep source view active during fade out phase
        // Activate target view immediately so it can render during fade in phase
        auto targetViewPtr = GetView(transitionTargetView);
        if(targetViewPtr) {
            targetViewPtr->SetActive(true);
            // std::cout << "[ViewManager] Target view '" << transitionTargetView << "' activated" << std::endl;
        }

        // Source view stays active until transition is complete
        auto sourceViewPtr = GetView(transitionSourceView);
        if(sourceViewPtr) {
           //  std::cout << "[ViewManager] Source view '" << transitionSourceView << "' remains active for fade out" << std::endl;
        }
    }

    void ViewManager::UpdateTransition(float deltaTime) {
        if (!isTransitioning) return;

        // std::cout << "[ViewManager] UpdateTransition: deltaTime=" << deltaTime << ", progress=" << transitionProgress << std::endl;

        // Transition speed (complete in 2.0 seconds for better visibility)
        float transitionSpeed = 0.5f;
        transitionProgress += deltaTime * transitionSpeed;

        // std::cout << "[ViewManager] UpdateTransition: new progress=" << transitionProgress << std::endl;

        if (transitionProgress >= 1.0f) {
            // Transition complete
            // std::cout << "[ViewManager] Transition complete!" << std::endl;
            transitionProgress = 1.0f;
            isTransitioning = false;

            // Deactivate source view
            if(!transitionSourceView.empty()) {
                auto sourceViewPtr = GetView(transitionSourceView);
                if(sourceViewPtr) {
                    sourceViewPtr->SetActive(false);
                    // std::cout << "[ViewManager] Source view '" << transitionSourceView << "' deactivated" << std::endl;
                }
            }

            // Target view is already active, just update current view
            currentView = transitionTargetView;
            // std::cout << "[ViewManager] Current view is now: " << currentView << std::endl;
        }
    }


    void ViewManager::SetRenderTarget(std::shared_ptr<NativeWindow> window) {
        renderWindow = window;

        // Update all existing views with current window dimensions
        if(renderWindow) {
            const auto& props = renderWindow->GetProperties();
            UpdateViewDimensions(props.width, props.height);
        }
    }

    void ViewManager::SetRenderingAPI(std::shared_ptr<Graphics::IRenderingAPI> api) {
        renderingAPI = api;
    }

    void ViewManager::RenderViews(Graphics::IRenderingAPI& api) {
        if(!renderWindow || !renderWindow->IsValid()) {
            return;
        }

        // Make the window's rendering context current
        renderWindow->MakeContextCurrent();

        // Views are updated via resize callback, no need to update every frame

        // Render the appropriate view(s)
        if(isTransitioning) {
            // std::cout << "[ViewManager] Rendering transition - progress: " << transitionProgress << std::endl;

            // Two-phase transition: fade out first half, fade in second half
            if(transitionProgress < 0.5f) {
                // First half: fade out old view
                // std::cout << "[ViewManager] Fade out phase" << std::endl;
                if(!transitionSourceView.empty()) {
                    auto sourceViewPtr = GetView(transitionSourceView);
                    if(sourceViewPtr && sourceViewPtr->IsActive() && sourceViewPtr->IsVisible()) {
                        // std::cout << "[ViewManager] Rendering source view: " << transitionSourceView << std::endl;
                        sourceViewPtr->Render(api);
                    }
                } else {
                    // If no source view, just clear with black
                    // std::cout << "[ViewManager] No source view to render, clearing screen" << std::endl;
                    api.Clear();
                }

                // Render black overlay on top for fade out effect
                float fadeOutProgress = transitionProgress * 2.0f; // 0.0 to 1.0 over first half
                float overlayAlpha = fadeOutProgress;

                // std::cout << "[ViewManager] Fade out overlay alpha: " << overlayAlpha << std::endl;
                const auto& props = renderWindow->GetProperties();
                // std::cout << "[ViewManager] Window properties: " << props.width << "x" << props.height << std::endl;
                api.Begin2D(props.width, props.height);
                api.DrawRect(0, 0, props.width, props.height, new ::Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, overlayAlpha));
                api.End2D();
            } else {
                // Second half: fade in new view
                // std::cout << "[ViewManager] Fade in phase" << std::endl;
                auto targetViewPtr = GetView(transitionTargetView);
                if(targetViewPtr && targetViewPtr->IsActive() && targetViewPtr->IsVisible()) {
                    // std::cout << "[ViewManager] Rendering target view: " << transitionTargetView << std::endl;
                    targetViewPtr->Render(api);
                } else {
                    // If no target view, just clear with black
                    // std::cout << "[ViewManager] No target view to render, clearing screen" << std::endl;
                    api.Clear();
                }

                // Render black overlay on top for fade in effect
                float fadeInProgress = (transitionProgress - 0.5f) * 2.0f; // 0.0 to 1.0 over second half
                float overlayAlpha = 1.0f - fadeInProgress;

                // std::cout << "[ViewManager] Fade in overlay alpha: " << overlayAlpha << std::endl;
                const auto& props = renderWindow->GetProperties();
                // std::cout << "[ViewManager] Window properties: " << props.width << "x" << props.height << std::endl;
                api.Begin2D(props.width, props.height);
                api.DrawRect(0, 0, props.width, props.height, new ::Engine::Graphics::RGBA(0.0f, 0.0f, 0.0f, overlayAlpha));
                api.End2D();
            }
        } else {
            // Normal rendering: render all active views
            (void)renderWindow->GetProperties(); // For debugging
            for(auto& [name, view] : views) {
                if(view && view->IsActive() && view->IsVisible()) {
                    view->Render(api);
                }
            }

            // Render overlays on top
            for(auto& [name, overlay] : overlays) {
                if(overlay && overlay->IsActive() && overlay->IsVisible()) {
                    overlay->Render(api);
                }
            }
        }

        // Present the frame
        api.SwapBuffers();
    }

    void ViewManager::ShowOverlay(const std::string& name, ViewPtr overlay) {
        if(overlay) {
            overlays[name] = overlay;
            overlay->SetActive(true);
            overlay->SetViewManager(this);

            // Set window dimensions if available
            if(renderWindow) {
                const auto& props = renderWindow->GetProperties();
                overlay->SetWindowDimensions(props.width, props.height);
            }
        }
    }

    void ViewManager::HideOverlay(const std::string& name) {
        auto it = overlays.find(name);
        if(it != overlays.end() && it->second) {
            it->second->SetActive(false);
            // DON'T clear key bindings - let the isActive check handle it
            overlays.erase(it);
        }
    }

    void ViewManager::HideAllOverlays() {
        for(auto& [name, overlay] : overlays) {
            if(overlay) {
                overlay->SetActive(false);
                // DON'T clear key bindings - let the isActive check handle it
            }
        }
        overlays.clear();
    }

    bool ViewManager::HasActiveOverlay() const {
        for(const auto& [name, overlay] : overlays) {
            if(overlay && overlay->IsActive() && overlay->IsVisible()) {
                return true;
            }
        }
        return false;
    }

    void ViewManager::UpdateViewDimensions(int width, int height) {
        if(renderingAPI) {
            renderingAPI->SetViewport(width, height);
        }

        // Update the NativeWindow properties so context.GetWidth/Height() returns correct values
        if(renderWindow) {
            renderWindow->UpdateWindowProperties(width, height);
        }

        for(auto& [name, view] : views) {
            if(view) {
                view->SetWindowDimensions(width, height);
            }
        }

        // Update overlay dimensions too
        for(auto& [name, overlay] : overlays) {
            if(overlay) {
                overlay->SetWindowDimensions(width, height);
            }
        }
    }

    Engine& ViewManager::GetEngine() const {
        return Engine::GetInstance();
    }
}
