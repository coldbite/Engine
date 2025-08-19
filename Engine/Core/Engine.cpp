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

        threadPool      = std::make_unique<ThreadPool>();
        lastFrameTime   = std::chrono::high_resolution_clock::now();
        frameStartTime  = lastFrameTime;
        lastFPSUpdate   = lastFrameTime;
        
        InitializeFrameTiming();

        InitEvent initEvent;
        eventDispatcher.Dispatch(initEvent);

        isInitialized = true;
        return true;
    }

    void Engine::Run() {
        if(!isInitialized.load()) {
            throw CoreException("Engine not initialized! Call Initialize() first.");
            return;
        }

        while(!shouldStop.load()) {
            frameStartTime = std::chrono::high_resolution_clock::now();
            
            // Calculate delta time
            deltaTime = std::chrono::duration<float>(frameStartTime - lastFrameTime).count();
            lastFrameTime = frameStartTime;
            
            // Always update and render - FPS limiting happens after rendering
            Update();
            FixedUpdate();

            // FPS limiting with precise timing
            if(fpsLimitEnabled) {
                auto frameEndTime = std::chrono::high_resolution_clock::now();
                auto frameDuration = std::chrono::duration<float>(frameEndTime - frameStartTime).count();
                
                if(frameDuration < targetFrameTime) {
                    float sleepTime = targetFrameTime - frameDuration;
                    std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            
            // Update FPS counter
            frameCount++;
            auto now = std::chrono::high_resolution_clock::now();
            auto fpsElapsed = std::chrono::duration<float>(now - lastFPSUpdate).count();
            if(fpsElapsed >= 1.0f) {
                currentFPS = static_cast<int>(frameCount / fpsElapsed);
                frameCount = 0;
                lastFPSUpdate = now;
            }
        }
    }

    void Engine::Update() {
        UpdateEvent updateEvent(deltaTime);
        eventDispatcher.Dispatch(updateEvent);
    }

    void Engine::FixedUpdate() {
        RenderEvent renderEvent;

        threadPool->Enqueue([this, renderEvent]() {
            eventDispatcher.Dispatch(renderEvent);
        });
    }

    void Engine::Shutdown() {
        if(!isInitialized.load()) {
            return;
        }

        shouldStop = true;

        ShutdownEvent shutdownEvent;
        eventDispatcher.Dispatch(shutdownEvent);

        if(threadPool) {
            threadPool->Stop();
            threadPool.reset();
        }

        isInitialized = false;
    }

    void Engine::InitializeFrameTiming() {
        if(HasOption(EngineOption::FRAMERATE_LIMIT_ENABLED)) {
            fpsLimitEnabled = GetOption(EngineOption::FRAMERATE_LIMIT_ENABLED, true);
        }
        
        if(HasOption(EngineOption::FRAMERATE_LIMIT_VALUE)) {
            targetFPS = GetOption(EngineOption::FRAMERATE_LIMIT_VALUE, 60);
            if(targetFPS <= 0) targetFPS = 60;
            targetFrameTime = 1.0f / static_cast<float>(targetFPS);
        }
        
        std::cout << "[Engine] Frame timing initialized - FPS Limit: " 
                  << (fpsLimitEnabled ? "ON (" + std::to_string(targetFPS) + " FPS)" : "OFF") << std::endl;
    }

    float Engine::GetDeltaTime() const {
        return deltaTime;
    }

    int Engine::GetCurrentFPS() const {
        return currentFPS;
    }
}
