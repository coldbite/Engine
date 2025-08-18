#include "Game.h"
#include "Exceptions/CoreException.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace Engine {
    Game::Game() : Engine(), isInitialized(false) {
        renderManager = std::make_unique<RenderManager>();
    }

    Game::~Game() {
        Shutdown();
    }

    bool Game::Initialize() {
        if (isInitialized) {
            return true;
        }

        SetupEventHandlers();

        if (!Engine::Initialize()) {
            throw CoreException("Failed to initialize engine!");
            return false;
        }

        isInitialized = true;
        return true;
    }

    void Game::Run() {
        if (!isInitialized) {
            return;
        }

        std::thread engineThread([this]() {
            Engine::Run();
        });

        std::thread inputThread([this]() {
            std::cin.get();
            RequestStop();
        });


        engineThread.join();
        inputThread.join();
    }

    void Game::Shutdown() {
        if (!isInitialized) {
            return;
        }

        OnShutdown();

        Engine::Shutdown();

        isInitialized = false;
    }

    void Game::SetupEventHandlers() {
        SubscribeToEvent<InitEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnInitEvent(event);
                OnInit();
            }
        );

        SubscribeToEvent<UpdateEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                const UpdateEvent& updateEvent = static_cast<const UpdateEvent&>(event);
                renderManager->OnUpdateEvent(event);
                OnUpdate(updateEvent.GetDeltaTime());
            }
        );

        SubscribeToEvent<RenderEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnRenderEvent(event);
                OnRender();
            }
        );

        SubscribeToEvent<ShutdownEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnShutdownEvent(event);
                OnShutdown();
            }
        );
    }

}
