#pragma once

#include "Event.h"
#include <iostream>
#include <string>

namespace Engine {
    class RenderManager {
    public:
        RenderManager();
        ~RenderManager();
        
        void Initialize();
        void Shutdown();
        
        void OnRenderEvent(const IEvent& event);
        void OnUpdateEvent(const IEvent& event);
        void OnInitEvent(const IEvent& event);
        void OnShutdownEvent(const IEvent& event);
        
    private:
        void RenderFrame(RenderEvent::RenderAPI api);
        std::string GetAPIName(RenderEvent::RenderAPI api);
        
        bool isInitialized;
        int frameCount;
    };
}