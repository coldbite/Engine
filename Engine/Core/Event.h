#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace Engine {
    class IEvent {
    public:
        virtual ~IEvent() = default;
        virtual std::type_index GetType() const = 0;
    };

    template<typename T>
    class Event : public IEvent {
    public:
        std::type_index GetType() const override {
            return std::type_index(typeid(T));
        }
    };

    class RenderEvent : public Event<RenderEvent> {
    public:
        enum class RenderAPI {
            OpenGL,
            DirectX,
            Vulkan
        };

        RenderEvent(RenderAPI api) : api(api) {}
        RenderAPI GetAPI() const { return api; }

    private:
        RenderAPI api;
    };

    class UpdateEvent : public Event<UpdateEvent> {
    public:
        UpdateEvent(float deltaTime) : deltaTime(deltaTime) {}
        float GetDeltaTime() const { return deltaTime; }

    private:
        float deltaTime;
    };

    class InitEvent : public Event<InitEvent> {};
    class ShutdownEvent : public Event<ShutdownEvent> {};

    using EventHandler = std::function<void(const IEvent&)>;

    class EventDispatcher {
    public:
        template<typename T>
        void Subscribe(EventHandler handler) {
            handlers[std::type_index(typeid(T))].push_back(handler);
        }

        void Dispatch(const IEvent& event) {
            auto it = handlers.find(event.GetType());
            if (it != handlers.end()) {
                for (auto& handler : it->second) {
                    handler(event);
                }
            }
        }

        template<typename T>
        void Unsubscribe() {
            handlers.erase(std::type_index(typeid(T)));
        }

    private:
        std::unordered_map<std::type_index, std::vector<EventHandler>> handlers;
    };
}