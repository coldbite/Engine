#include "Masterball.h"

using Engine::Config;
using Engine::EngineOption;

void Masterball::OnInit() {
    /* Bind Options */
    AddOptions<MB>();

    /* Load Configuration */
    Config::Load("Game.conf");

    if(GetOption<MB, bool>(MB::DEBUGGING, false)) {
        std::cout << "[Masterball] Debug ON" << std::endl;
    } else {
        std::cout << "[Masterball] Debug OFF" << std::endl;
    }

    /* Set the Rendering-API */
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
        SetOption<EngineOption, int>(EngineOption::RESOLUTION_MODE, Config::GetInt("Render.FullscreenMode", 0));
    }

    /* Set Display Specs */
    if(Config::Has("Render.Resolution.Width")) {
        SetOption<EngineOption, int>(EngineOption::RESOLUTION_WIDTH, Config::GetInt("Render.Resolution.Width", 768));
    }

    if(Config::Has("Render.Resolution.Height")) {
        SetOption<EngineOption, int>(EngineOption::RESOLUTION_HEIGHT, Config::GetInt("Render.Resolution.Height", 1024));
    }

    if(Config::Has("Render.Resolution.Hertz")) {
        SetOption<EngineOption, int>(EngineOption::RESOLUTION_HERTZ, Config::GetInt("Render.Resolution.Hertz", 60));
    }

    if(Config::Has("Render.Resolution.Scale")) {
        SetOption<EngineOption, float>(EngineOption::RESOLUTION_SCALE, Config::GetFloat("Render.Resolution.Scale", 1.0));
    }

    // Show UI/Loading

    // Set up loading progress callback
    map.OnLoading([](const std::string& message, int actual, int total, float percentage) {
        std::cout << "[Masterball] Loading Progress: " << message
                  << " (" << actual << "/" << total << ") "
                  << percentage << "%" << std::endl;
        // Update UI/Loading
    });

    // Set up loaded callback
    map.OnLoaded([]() {
        // Hide UI/Loading
        // Show UI/Playing
        std::cout << "[Masterball] Map fully loaded and ready!" << std::endl;
    });

    // Load the map
    map.Load("Waterworld");
}

void Masterball::OnShutdown() {
    // std::cout << "[Masterball] Game shutting down!" << std::endl;
}

void Masterball::OnUpdate(float /*deltaTime*/) {
    // std::cout << "[Masterball] Update - Delta: " << deltaTime << "s" << std::endl;
}

void Masterball::OnRender() {
    // std::cout << "[Masterball] Custom render logic" << std::endl;
}
