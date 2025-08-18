#include "Engine.h"
#include "Exceptions/CoreException.h"
#include <iostream>
#include <thread>

namespace Engine {
    Engine& Engine::GetInstance() {
        static Engine instance;
        return instance;
    }

    bool Engine::Initialize() {
        if(isInitialized.load()) {
            return true;
        }

        threadPool = std::make_unique<ThreadPool>();
        lastFrameTime = std::chrono::high_resolution_clock::now();

        InitEvent initEvent;
        eventDispatcher.Dispatch(initEvent);

        isInitialized = true;
        return true;
    }

    void Engine::Run() {
        if (!isInitialized.load()) {
            throw CoreException("Engine not initialized! Call Initialize() first.");
            return;
        }

        while (!shouldStop.load()) {
            Update();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();

            if (deltaTime >= targetFrameTime) {
                FixedUpdate();
                lastFrameTime = currentTime;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void Engine::Update() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();

        UpdateEvent updateEvent(deltaTime);
        eventDispatcher.Dispatch(updateEvent);
    }

    void Engine::FixedUpdate() {
        RenderEvent renderEvent(RenderEvent::RenderAPI::OpenGL);

        threadPool->Enqueue([this, renderEvent]() {
            eventDispatcher.Dispatch(renderEvent);
        });
    }

    void Engine::Shutdown() {
        if (!isInitialized.load()) {
            return;
        }

        shouldStop = true;

        ShutdownEvent shutdownEvent;
        eventDispatcher.Dispatch(shutdownEvent);

        if (threadPool) {
            threadPool->Stop();
            threadPool.reset();
        }

        isInitialized = false;
    }
}
