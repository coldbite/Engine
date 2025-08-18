#pragma once

#include "Event.h"
#include "ThreadPool.h"
#include "Settings/Config.h"
#include "CommandLine/Arguments.h"
#include <memory>
#include <stdexcept>
#include <atomic>
#include <chrono>

namespace Engine {
    /* Simple Alias */
    using Config    = Settings::Config;
    using Arguments = CommandLine::Arguments;

    /* Settings & Options */
    enum class EngineOption {
        DEBUGGING               = 1000,
        RENDERER                = 1002,
        VSYNC                   = 1003,
        RESOLUTION_MODE         = 2001,
        RESOLUTION_HEIGHT       = 2002,
        RESOLUTION_WIDTH        = 2003,
        RESOLUTION_HERTZ        = 2004,
        RESOLUTION_SCALE        = 2005,
        FRAMERATE_LIMIT_ENABLED = 3001,
        FRAMERATE_LIMIT_VALUE   = 3002,
        GAME_TITLE              = 4001
    };

    class Engine {
    public:
        static Engine& GetInstance();

        bool Initialize();
        void Run();
        void Shutdown();

        EventDispatcher& GetEventDispatcher() {
                return eventDispatcher;
        }

        ThreadPool& GetThreadPool() {
                return *threadPool;
        }

        void RequestStop() {
                shouldStop = true;
        }

        bool ShouldStop() const {
                return shouldStop;
        }

        template<typename T>
        void SubscribeToEvent(EventHandler handler) {
            eventDispatcher.Subscribe<T>(handler);
        }

        void DispatchEvent(const IEvent& event) {
            eventDispatcher.Dispatch(event);
        }

    protected:
        Engine() = default;
        virtual ~Engine() = default;
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void Update();
        void FixedUpdate();

        EventDispatcher eventDispatcher;
        std::unique_ptr<ThreadPool> threadPool;
        std::atomic<bool> shouldStop{false};
        std::atomic<bool> isInitialized{false};

        std::chrono::high_resolution_clock::time_point lastFrameTime;
        const float targetFrameTime = 1.0f / 60.0f;
    };
}
