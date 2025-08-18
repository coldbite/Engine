#include "RenderManager.h"
#include <thread>
#include <algorithm>

namespace Engine {
    RenderManager::RenderManager() : isInitialized(false), frameCount(0) {
        /* Do Nothing */
    }

    RenderManager::~RenderManager() {
        Shutdown();
    }

    void RenderManager::Initialize() {
        if(isInitialized) {
            return;
        }

        std::cout << "[RenderManager] Initializing..." << std::endl;
        isInitialized   = true;
        frameCount      = 0;
    }

    void RenderManager::Shutdown() {
        if(!isInitialized) {
            return;
        }

        std::cout << "[RenderManager] Shutting down... (Rendered " << frameCount << " frames)" << std::endl;
        ClearRenderables();
        isInitialized = false;
    }

    void RenderManager::OnRenderEvent(const IEvent& event) {
        if(!isInitialized) {
            return;
        }

        const RenderEvent& renderEvent = static_cast<const RenderEvent&>(event);
        RenderFrame();
    }

    void RenderManager::OnUpdateEvent(const IEvent& event) {
        if(!isInitialized) {
            return;
        }

        const UpdateEvent& updateEvent = static_cast<const UpdateEvent&>(event);
    }

    void RenderManager::OnInitEvent(const IEvent& event) {
        Initialize();
    }

    void RenderManager::OnShutdownEvent(const IEvent& event) {
        Shutdown();
    }

    void RenderManager::RenderFrame() {
        frameCount++;

        // Prepare all renderables for rendering (without actual API calls yet)
        PrepareRenderables();

        std::thread::id threadId = std::this_thread::get_id();
        /*std::cout << "[RenderManager] Frame " << frameCount
                  << " - Rendering " << renderables.size() << " objects with " << GetAPIName(api)
                  << " API (Thread: " << threadId << ")" << std::endl;*/

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    void RenderManager::AddRenderable(RenderablePtr renderable) {
        if(!renderable) {
            std::cout << "[RenderManager] Warning: Attempted to add null renderable" << std::endl;
            return;
        }

        // Check if object with same name already exists
        const std::string& name = renderable->GetName();

        if(renderableMap.find(name) != renderableMap.end()) {
            std::cout << "[RenderManager] Warning: Renderable '" << name << "' already exists, replacing..." << std::endl;
            RemoveRenderable(name);
        }

        renderables.push_back(renderable);
        renderableMap[name] = renderable;

        std::cout << "[RenderManager] Added renderable: " << name << " (Total: " << renderables.size() << ")" << std::endl;
    }

    void RenderManager::RemoveRenderable(const std::string& name) {
        auto mapIt = renderableMap.find(name);

        if(mapIt == renderableMap.end()) {
            std::cout << "[RenderManager] Warning: Renderable '" << name << "' not found for removal" << std::endl;
            return;
        }

        RenderablePtr renderable = mapIt->second;

        // Remove from vector
        auto vecIt = std::find(renderables.begin(), renderables.end(), renderable);

        if(vecIt != renderables.end()) {
            renderables.erase(vecIt);
        }

        // Remove from map
        renderableMap.erase(mapIt);

        std::cout << "[RenderManager] Removed renderable: " << name << " (Total: " << renderables.size() << ")" << std::endl;
    }

    void RenderManager::RemoveRenderable(RenderablePtr renderable) {
        if(!renderable) {
            return;
        }

        RemoveRenderable(renderable->GetName());
    }

    void RenderManager::ClearRenderables() {
        std::cout << "[RenderManager] Clearing all renderables (Count: " << renderables.size() << ")" << std::endl;
        renderables.clear();
        renderableMap.clear();
    }

    RenderablePtr RenderManager::GetRenderable(const std::string& name) {
        auto it = renderableMap.find(name);
        return (it != renderableMap.end()) ? it->second : nullptr;
    }

    const std::vector<RenderablePtr>& RenderManager::GetRenderables() const {
        return renderables;
    }

    size_t RenderManager::GetRenderableCount() const {
        return renderables.size();
    }

    void RenderManager::PrepareRenderables() {
        // Call PrepareForRendering on all visible renderables
        for(const auto& renderable : renderables) {
            if(renderable && renderable->IsVisible()) {
                renderable->PrepareForRendering();
            }
        }
    }
}
