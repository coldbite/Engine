#include "RenderManager.h"
#include <thread>

namespace Engine {
    RenderManager::RenderManager() : isInitialized(false), frameCount(0) {
    }

    RenderManager::~RenderManager() {
        Shutdown();
    }

    void RenderManager::Initialize() {
        if (isInitialized) {
            return;
        }
        
        std::cout << "[RenderManager] Initializing..." << std::endl;
        isInitialized = true;
        frameCount = 0;
    }

    void RenderManager::Shutdown() {
        if (!isInitialized) {
            return;
        }
        
        std::cout << "[RenderManager] Shutting down... (Rendered " << frameCount << " frames)" << std::endl;
        isInitialized = false;
    }

    void RenderManager::OnRenderEvent(const IEvent& event) {
        if (!isInitialized) {
            return;
        }
        
        const RenderEvent& renderEvent = static_cast<const RenderEvent&>(event);
        RenderFrame(renderEvent.GetAPI());
    }

    void RenderManager::OnUpdateEvent(const IEvent& event) {
        if (!isInitialized) {
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

    void RenderManager::RenderFrame(RenderEvent::RenderAPI api) {
        frameCount++;
        
        std::thread::id threadId = std::this_thread::get_id();
        std::cout << "[RenderManager] Frame " << frameCount 
                  << " - Rendering with " << GetAPIName(api) 
                  << " API (Thread: " << threadId << ")" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::string RenderManager::GetAPIName(RenderEvent::RenderAPI api) {
        switch (api) {
            case RenderEvent::RenderAPI::OpenGL:
                return "OpenGL";
            case RenderEvent::RenderAPI::DirectX:
                return "DirectX";
            case RenderEvent::RenderAPI::Vulkan:
                return "Vulkan";
            default:
                return "Unknown";
        }
    }
}