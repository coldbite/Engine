#include "Game.h"
#include "Input/InputManager.h"
#include "Exceptions/CoreException.h"
#include "../Graphics/OpenGL/OpenGL.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

#ifdef _WIN32
    #include <GL/gl.h>
#endif

namespace Engine {
    Game::Game() : Engine(), isInitialized(false) {
        viewManager     = std::make_unique<ViewManager>(this);
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

        std::string icon = "Assets/Logo.ico";

        if(HasOption(EngineOption::GAME_ICON)) {
            icon = GetOption(EngineOption::GAME_ICON, icon);
        }

        windowProps.icon = icon;

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

        // Connect InputManager with NativeWindow
        mainWindow->SetKeyCallback([](int key, int action) {
            Input::GetInput().OnKeyEvent(key, action);
        });
        
        mainWindow->SetMouseButtonCallback([](int button, int action, float x, float y) {
            Input::GetInput().OnMouseButtonEvent(button, action, x, y);
        });
        
        mainWindow->SetMouseMoveCallback([](float x, float y) {
            Input::GetInput().OnMouseMoveEvent(x, y);
        });
        
        mainWindow->SetMouseScrollCallback([](float deltaX, float deltaY) {
            Input::GetInput().OnMouseScrollEvent(deltaX, deltaY);
        });

        std::string renderer = "OpenGL";

        if(HasOption(EngineOption::RENDERER)) {
            renderer = GetOption(EngineOption::RENDERER, renderer);
        }

        if(!mainWindow->SetupRenderingContext(renderer)) {
            std::cout << "[Game] Warning: Failed to setup rendering context for " << renderer << std::endl;
        }

        // Create rendering API instance
        renderingAPI = std::make_shared<Graphics::OpenGL::OpenGL>();
        if (!renderingAPI->Init(mainWindow)) {
            throw CoreException("Failed to initialize rendering API!");
            return false;
        }

        // Connect ViewManager to the window and rendering API
        viewManager->SetRenderTarget(mainWindow);
        viewManager->SetRenderingAPI(renderingAPI);

        // Set up window resize callback to update view dimensions
        mainWindow->SetResizeCallback([this](int width, int height) {
            std::cout << "[Game] Window resized to: " << width << "x" << height << std::endl;
            if (viewManager) {
                viewManager->UpdateViewDimensions(width, height);
            }
        });

        // Set up window close callback to shutdown the game
        mainWindow->SetCloseCallback([this]() {
            Exit();
        });

        // Ensure VSync is applied after context setup
        if(windowProps.vsync && mainWindow->IsValid()) {
            std::cout << "[Game] Applying VSync setting: ON" << std::endl;
            mainWindow->SetVSync(true);
        }

        mainWindow->Show();
        isInitialized = true;
        return true;
    }

    void Game::Exit() {
        std::cout << "[Game] Window close requested - shutting down..." << std::endl;
        RequestStop();
        Engine::RequestStop();
        std::cout << "[Game] Stop requests sent" << std::endl;
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
                
                // Update InputManager to handle key state changes
                Input::GetInput().Update();
            }

            // Render in main thread (OpenGL requirement)
            if(viewManager && mainWindow && mainWindow->IsValid() && renderingAPI) {
                viewManager->RenderViews(*renderingAPI);
            }

            // Small sleep for main thread timing
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }

        std::cout << "[Game] Main loop exited" << std::endl;

        // Wait for threads to finish after stop is requested
        std::cout << "[Game] Waiting for engine thread to finish..." << std::endl;
        if(engineThread.joinable()) {
            engineThread.join();
        }
        std::cout << "[Game] Engine thread finished" << std::endl;

        if(hasInputThread && inputThread.joinable()) {
            std::cout << "[Game] Detaching input thread (it's waiting for stdin)..." << std::endl;
            inputThread.detach(); // Don't wait for input thread - it's blocking on stdin
            std::cout << "[Game] Input thread detached" << std::endl;
        }

        // Call shutdown after the main loop ends
        std::cout << "[Game] Calling Shutdown()..." << std::endl;
        Shutdown();
    }

    void Game::Shutdown() {
        if(!isInitialized) {
            return;
        }

        // Set to false immediately to prevent double shutdown
        isInitialized = false;

        std::cout << "[Game] Shutting down..." << std::endl;
        std::cout << "[Game] Calling OnShutdown()..." << std::endl;
        OnShutdown();
        std::cout << "[Game] OnShutdown() finished" << std::endl;

        Engine::Shutdown();
    }

    void Game::SetupEventHandlers() {
        SubscribeToEvent<InitEvent>(
            [this](const IEvent& /*event*/) {
                OnInit();
            }
        );

        SubscribeToEvent<RenderEvent>(
            [this](const IEvent& /*event*/) {
                // Rendering is now handled directly in main loop via viewManager->RenderViews()
            }
        );

        SubscribeToEvent<UpdateEvent>(
            [this, &viewManager = this->viewManager](const IEvent& event) {
                const UpdateEvent& updateEvent = static_cast<const UpdateEvent&>(event);
                viewManager->UpdateViews(updateEvent.GetDeltaTime());
            }
        );

        SubscribeToEvent<ShutdownEvent>(
            [this](const IEvent& /*event*/) {
                viewManager->HideAllViews();
                // OnShutdown() is called explicitly in Game::Shutdown(), not here
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
