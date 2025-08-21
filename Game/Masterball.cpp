#include "Masterball.h"
#include "Event.h"
#include "../Engine/Core/Settings/Config.h"

using Engine::Config;
using Engine::EngineOption;
using Engine::ViewChangeEvent;

void Masterball::OnInit() {
    /* Bind Options */
    AddOptions<MB>();

    /* Some Engine Settings */
    SetOption<EngineOption, std::string>(EngineOption::GAME_TITLE,  "Masterball (Beta)");
    SetOption<EngineOption, std::string>(EngineOption::GAME_ICON,   "Assets/Logo.ico");

    /* Load Configuration */
    if(GetOption<MB, std::string>(MB::TYPE, "CLIENT") == "SERVER") {
        std::cout << "[Masterball] Starting dedicated Server" << std::endl;
        // @ToDo not exists, create File with defaults
        Config::Load("Server.conf");
    } else {
        // @ToDo not exists, create File with defaults
        Config::Load("Game.conf");
    }

    if(GetOption<MB, bool>(MB::DEBUGGING, false)) {
        std::cout << "[Masterball] Debug ON" << std::endl;
    } else {
        std::cout << "[Masterball] Debug OFF" << std::endl;
    }

    /* Set the Rendering-API */
    if(GetOption<MB, std::string>(MB::TYPE, "CLIENT") == "CLIENT") {
        if(Config::Has("Render.Renderer")) {
            std::string renderer = Config::GetString("Render.Renderer", "OpenGL");
            std::cout << "[Masterball] Config Renderer: " << renderer << std::endl;
            SetOption<EngineOption, std::string>(EngineOption::RENDERER, renderer);
        } else {
            throw std::runtime_error("Renderer initialization failed");
        }

        /* Enable Vertical-Synchronisation */
        if(Config::Has("Render.VSync")) {
            SetOption<EngineOption, bool>(EngineOption::VSYNC, Config::GetBool("Render.VSync", true));
        }

        /* Enable FPS-Limit */
        if(Config::Has("Render.FrameRateLimiterEnable")) {
            SetOption<EngineOption, bool>(EngineOption::FRAMERATE_LIMIT_ENABLED, Config::GetBool("Render.FrameRateLimiterEnable", false));
        }

        /* Set FPS-Limit */
        if(Config::Has("Render.FrameRateLimit")) {
            SetOption<EngineOption, int>(EngineOption::FRAMERATE_LIMIT_ENABLED, Config::GetInt("Render.FrameRateLimit", 240));
        }

        /* Set Screen-Mode */
        if(Config::Has("Render.FullscreenMode")) {
            int fullscreenMode = Config::GetInt("Render.FullscreenMode", 0);
            std::cout << "[Masterball] Config FullscreenMode: " << fullscreenMode << std::endl;
            SetOption<EngineOption, int>(EngineOption::RESOLUTION_MODE, fullscreenMode);
        } else {
            std::cout << "[Masterball] Config FullscreenMode not found!" << std::endl;
        }

        /* Set Display Specs */
        if(Config::Has("Render.Resolution.Width")) {
            int width = Config::GetInt("Render.Resolution.Width", 768);
            std::cout << "[Masterball] Config Resolution Width: " << width << std::endl;
            SetOption<EngineOption, int>(EngineOption::RESOLUTION_WIDTH, width);
        } else {
            std::cout << "[Masterball] Config Resolution.Width not found!" << std::endl;
        }

        if(Config::Has("Render.Resolution.Height")) {
            int height = Config::GetInt("Render.Resolution.Height", 1024);
            std::cout << "[Masterball] Config Resolution Height: " << height << std::endl;
            SetOption<EngineOption, int>(EngineOption::RESOLUTION_HEIGHT, height);
        } else {
            std::cout << "[Masterball] Config Resolution.Height not found!" << std::endl;
        }

        if(Config::Has("Render.Resolution.Hertz")) {
            SetOption<EngineOption, int>(EngineOption::RESOLUTION_HERTZ, Config::GetInt("Render.Resolution.Hertz", 60));
        }

        if(Config::Has("Render.Resolution.Scale")) {
            SetOption<EngineOption, float>(EngineOption::RESOLUTION_SCALE, Config::GetFloat("Render.Resolution.Scale", 1.0));
        }
    }

    /* Register Fonts */
    RegisterFont("Sansation", "../Game/Assets/Fonts/Sansation-Regular.ttf");

    loading = std::make_shared<Loading>();
    playing = std::make_shared<Playing>();

    GetViewManager().RegisterView("Loading", loading);
    GetViewManager().RegisterView("Playing", playing);

    map.OnLoading([this](const std::string& message, int actual, int total, float percentage) {
        if(loading) {
            loading->OnUpdateProgress(message, actual, total, percentage);
        }
    });

    // Set up loaded callback
    map.OnLoaded([this]() {
        DispatchEvent(ViewChangeEvent("Playing", ::Engine::Transition::FADE));
    });

    // Received from Server
    std::string mapName = "Waterworld";
    loading->SetGameMode("Singleplayer");

    map.Load(mapName, [this](const std::string& name, const Config& mapConfig) {
        if(loading) {
            if(mapConfig.HasImpl("Name")) {
                loading->SetMapName(mapConfig.GetStringImpl("Name", name));
            } else {
                loading->SetMapName(name);
            }

            if(mapConfig.HasImpl("Loading.Background")) {
                std::string loadingBg = "Maps/" + name + "/" + mapConfig.GetStringImpl("Loading.Background", "");

                if(!loadingBg.empty()) {
                    loading->SetBackgroundImage(loadingBg);
                }
            }
        }
    });

    DispatchEvent(ViewChangeEvent("Loading"));
}

void Masterball::OnShutdown() {
    std::cout << "[Masterball] Game shutting down!" << std::endl;
}
