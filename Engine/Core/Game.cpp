#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace Engine {
    Game::Game() : isInitialized(false) {
        renderManager = std::make_unique<RenderManager>();
    }

    Game::~Game() {
        Shutdown();
    }

    bool Game::Initialize() {
        if (isInitialized) {
            return true;
        }

        Engine& engine = GetEngine();
        
        SetupEventHandlers();
        
        if (!engine.Initialize()) {
            std::cout << "Failed to initialize engine!" << std::endl;
            return false;
        }
        
        isInitialized = true;
        return true;
    }

    void Game::Run() {
        if (!isInitialized) {
            return;
        }

        Engine& engine = GetEngine();
        
        std::thread engineThread([&engine]() {
            engine.Run();
        });
        
        std::thread inputThread([&engine]() {
            std::cin.get();
            engine.RequestStop();
        });
        
        std::thread demoThread([this]() {
            RunDemo();
        });
        
        engineThread.join();
        inputThread.join();
        demoThread.join();
    }

    void Game::Shutdown() {
        if (!isInitialized) {
            return;
        }

        OnShutdown();
        
        Engine& engine = GetEngine();
        engine.Shutdown();
        
        isInitialized = false;
    }

    void Game::SetupEventHandlers() {
        Engine& engine = GetEngine();
        
        engine.SubscribeToEvent<InitEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnInitEvent(event);
                OnInit();
            }
        );
        
        engine.SubscribeToEvent<UpdateEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                const UpdateEvent& updateEvent = static_cast<const UpdateEvent&>(event);
                renderManager->OnUpdateEvent(event);
                OnUpdate(updateEvent.GetDeltaTime());
            }
        );
        
        engine.SubscribeToEvent<RenderEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnRenderEvent(event);
                OnRender();
            }
        );
        
        engine.SubscribeToEvent<ShutdownEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnShutdownEvent(event);
                OnShutdown();
            }
        );
    }

    void Game::RunDemo() {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        Engine& engine = GetEngine();
        
        RenderEvent openglEvent(RenderEvent::RenderAPI::OpenGL);
        engine.DispatchEvent(openglEvent);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        RenderEvent directxEvent(RenderEvent::RenderAPI::DirectX);
        engine.DispatchEvent(directxEvent);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        RenderEvent vulkanEvent(RenderEvent::RenderAPI::Vulkan);
        engine.DispatchEvent(vulkanEvent);
    }
}
