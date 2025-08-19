#include "ViewManager.h"
#include "../Graphics/OpenGL/OpenGL.h"
#include <iostream>
#include <algorithm>

namespace Engine {
    ViewManager::ViewManager() : currentView("") {
        /* Do Nothing */
    }

    void ViewManager::RegisterView(const std::string& name, ViewPtr view) {
        if(!view) {
            //std::cout << "[ViewManager] Warning: Attempted to register null view" << std::endl;
            return;
        }

        views[name] = view;
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

    void ViewManager::UpdateViews(float deltaTime) {
        // Update transition state
        UpdateTransition(deltaTime);
        
        for(auto& [name, view] : views) {
            if(view && view->IsActive()) {
                view->OnUpdate(deltaTime);
            }
        }
    }

    void ViewManager::OnViewChangeEvent(const ViewChangeEvent& event) {
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
        
        // Activate target view immediately so it can render during transition
        auto targetViewPtr = GetView(transitionTargetView);
        if(targetViewPtr) {
            targetViewPtr->SetActive(true);
        }
    }

    void ViewManager::UpdateTransition(float deltaTime) {
        if (!isTransitioning) return;

        // Transition speed (complete in 1.0 seconds)
        float transitionSpeed = 1.0f;
        transitionProgress += deltaTime * transitionSpeed;

        if (transitionProgress >= 1.0f) {
            // Transition complete
            transitionProgress = 1.0f;
            isTransitioning = false;

            // Deactivate source view
            if(!transitionSourceView.empty()) {
                auto sourceViewPtr = GetView(transitionSourceView);
                if(sourceViewPtr) {
                    sourceViewPtr->SetActive(false);
                }
            }

            // Target view is already active, just update current view
            currentView = transitionTargetView;
        }
    }

    void ViewManager::RenderViews() {
        if(!renderWindow || !renderWindow->IsValid()) {
            return;
        }

        // Make the window's rendering context current
        renderWindow->MakeContextCurrent();

        // Render the appropriate view(s)
        if(isTransitioning) {
            // Two-phase transition: fade out first half, fade in second half
            if(transitionProgress < 0.5f) {
                // First half: fade out old view
                if(!transitionSourceView.empty()) {
                    auto sourceViewPtr = GetView(transitionSourceView);
                    if(sourceViewPtr && sourceViewPtr->IsActive() && sourceViewPtr->IsVisible()) {
                        sourceViewPtr->Render();
                    }
                }
                
                // Black overlay that fades from transparent to opaque
                float fadeOutProgress = transitionProgress * 2.0f; // 0.0 to 1.0 over first half
                float overlayAlpha = fadeOutProgress;
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, 800, 0, 600, -1, 1);
                
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_DEPTH_TEST);
                
                glColor4f(0.0f, 0.0f, 0.0f, overlayAlpha);
                
                glBegin(GL_QUADS);
                    glVertex2f(0, 0);
                    glVertex2f(800, 0);
                    glVertex2f(800, 600);
                    glVertex2f(0, 600);
                glEnd();
                
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                
                glEnable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            } else {
                // Second half: fade in new view
                auto targetViewPtr = GetView(transitionTargetView);
                if(targetViewPtr && targetViewPtr->IsActive() && targetViewPtr->IsVisible()) {
                    targetViewPtr->Render();
                }
                
                // Black overlay that fades from opaque to transparent
                float fadeInProgress = (transitionProgress - 0.5f) * 2.0f; // 0.0 to 1.0 over second half
                float overlayAlpha = 1.0f - fadeInProgress;
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, 800, 0, 600, -1, 1);
                
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();
                
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_DEPTH_TEST);
                
                glColor4f(0.0f, 0.0f, 0.0f, overlayAlpha);
                
                glBegin(GL_QUADS);
                    glVertex2f(0, 0);
                    glVertex2f(800, 0);
                    glVertex2f(800, 600);
                    glVertex2f(0, 600);
                glEnd();
                
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                
                glEnable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            }
        } else {
            // Normal rendering: render all active views
            for(auto& [name, view] : views) {
                if(view && view->IsActive() && view->IsVisible()) {
                    view->Render();
                }
            }
        }

        // Present the frame
        renderWindow->SwapBuffers();
    }

    void ViewManager::SetRenderTarget(std::shared_ptr<NativeWindow> window) {
        renderWindow = window;
    }
}
