#include "Game.h"
#include "Exceptions/CoreException.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

namespace Engine {
    Game::Game() : Engine(), isInitialized(false) {
        renderManager   = std::make_unique<RenderManager>();
        viewManager     = std::make_unique<ViewManager>();
        mainWindow      = std::make_shared<NativeWindow>();
    }

    Game::~Game() {
        Shutdown();
    }

    bool Game::Initialize() {
        if(isInitialized) {
            return true;
        }

        SetupEventHandlers();

        if(!Engine::Initialize()) {
            throw CoreException("Failed to initialize engine!");
            return false;
        }

        // Create and setup main window FIRST, before anything else
        WindowProperties windowProps;

        std::string title = "Coldbite";

        if(HasOption(EngineOption::GAME_TITLE)) {
            title = GetOption(EngineOption::GAME_TITLE, title);
        }

        windowProps.title = title;

        if(HasOption(EngineOption::RESOLUTION_WIDTH) && HasOption(EngineOption::RESOLUTION_HEIGHT)) {
            windowProps.width   = GetOption(EngineOption::RESOLUTION_WIDTH, 800);
            windowProps.height  = GetOption(EngineOption::RESOLUTION_HEIGHT, 600);
        }

        if(HasOption(EngineOption::RESOLUTION_MODE)) {
            int resolutionMode = GetOption(EngineOption::RESOLUTION_MODE, 0);
            std::cout << "[Game] Resolution mode from config: " << resolutionMode << std::endl;

            switch(resolutionMode) {
                /* Window */
                case 0:
                    windowProps.resizable   = true;
                    std::cout << "[Game] Setting windowed mode" << std::endl;
                break;

                /* Windowed */
                case 1:
                    windowProps.resizable   = false;
                    // flags |= B_NOT_RESIZABLE | B_NOT_ZOOMABLE;
                    // look = B_NO_BORDER_WINDOW_LOOK;
                    std::cout << "[Game] Setting windowed-fullscreen mode" << std::endl;
                break;

                /* Fullscreen */
                case 2:
                    windowProps.fullscreen  = true;
                    std::cout << "[Game] Setting fullscreen mode" << std::endl;
                break;
            }
        }

        if(HasOption(EngineOption::VSYNC)) {
            windowProps.vsync   = GetOption(EngineOption::VSYNC, false);
        }

        std::cout << "[Game] Creating window - Size: " << windowProps.width << "x" << windowProps.height
                  << ", Fullscreen: " << (windowProps.fullscreen ? "true" : "false") << std::endl;

        if(!mainWindow->Create(windowProps)) {
            throw CoreException("Failed to create main window!");
            return false;
        }

        std::string renderer = "OpenGL";

        if(HasOption(EngineOption::RENDERER)) {
            renderer = GetOption(EngineOption::RENDERER, renderer);
        }

        if(!mainWindow->SetupRenderingContext(renderer)) {
            std::cout << "[Game] Warning: Failed to setup rendering context for " << renderer << std::endl;
        }

        // Connect ViewManager to the window for rendering
        viewManager->SetRenderTarget(mainWindow);
        
        // Ensure VSync is applied after context setup
        if(windowProps.vsync && mainWindow->IsValid()) {
            std::cout << "[Game] Applying VSync setting: ON" << std::endl;
            mainWindow->SetVSync(true);
        }
        
        mainWindow->Show();
        isInitialized = true;
        return true;
    }

    void Game::Run() {
        if(!isInitialized) {
            return;
        }

        std::thread engineThread([this]() {
            Engine::Run();
        });

        // Only create input thread if not in a pipe/redirect situation
        std::thread inputThread;
        bool hasInputThread = false;

        if(isatty(STDIN_FILENO)) {
            inputThread = std::thread([this]() {
                std::cin.get();
                RequestStop();
                Engine::RequestStop(); // Also stop the engine
            });

            hasInputThread = true;
        }

        // Keep main thread for window message pumping AND rendering (OpenGL context requirement)
        while(!shouldStop.load()) {
            // Process window events in main thread
            if(mainWindow && mainWindow->IsValid()) {
                mainWindow->PollEvents();
            }
            
            // Render in main thread (OpenGL requirement)
            if(viewManager && mainWindow && mainWindow->IsValid()) {
                viewManager->RenderViews();
            }

            // Small sleep for main thread timing
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }

        // Wait for threads to finish after stop is requested
        if(engineThread.joinable()) {
            engineThread.join();
        }

        if(hasInputThread && inputThread.joinable()) {
            inputThread.join();
        }
    }

    void Game::Shutdown() {
        if(!isInitialized) {
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

        SubscribeToEvent<RenderEvent>(
            [this, &renderManager = this->renderManager](const IEvent& event) {
                renderManager->OnRenderEvent(event);
                // Note: viewManager->RenderViews() is now called in main thread
            }
        );

        SubscribeToEvent<UpdateEvent>(
            [this, &viewManager = this->viewManager](const IEvent& event) {
                const UpdateEvent& updateEvent = static_cast<const UpdateEvent&>(event);
                viewManager->UpdateViews(updateEvent.GetDeltaTime());
            }
        );

        SubscribeToEvent<ShutdownEvent>(
            [this, &renderManager = this->renderManager, &viewManager = this->viewManager](const IEvent& event) {
                renderManager->OnShutdownEvent(event);
                viewManager->HideAllViews();
                OnShutdown();
            }
        );

        SubscribeToEvent<ViewChangeEvent>(
            [this, &viewManager = this->viewManager](const IEvent& event) {
                const ViewChangeEvent& viewEvent = static_cast<const ViewChangeEvent&>(event);
                viewManager->OnViewChangeEvent(viewEvent);
            }
        );
    }

}
