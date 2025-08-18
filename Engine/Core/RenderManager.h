#pragma once

#include "Event.h"
#include "Renderable.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

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

        // Rendering object management
        void AddRenderable(RenderablePtr renderable);
        void RemoveRenderable(const std::string& name);
        void RemoveRenderable(RenderablePtr renderable);
        void ClearRenderables();

        RenderablePtr GetRenderable(const std::string& name);
        const std::vector<RenderablePtr>& GetRenderables() const;
        size_t GetRenderableCount() const;

    private:
        void RenderFrame();
        void PrepareRenderables();

        bool isInitialized;
        int frameCount;

        std::vector<RenderablePtr> renderables;
        std::unordered_map<std::string, RenderablePtr> renderableMap;
    };
}
