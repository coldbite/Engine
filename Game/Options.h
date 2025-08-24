#pragma once
#include "../Engine/Graphics/RGBA.h"
#include <memory>

enum class MB {
    TYPE        = 1,
    DEBUGGING   = 2
};

/*
    Possible Values:
    TYPE        = SERVER, CLIENT
    DEBUGGING   = boolean
*/

// Global UI Colors - prevents memory leaks by reusing color objects
namespace UIColors {
    // Score table colors
    extern std::unique_ptr<Engine::Graphics::RGBA> headerBgColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> headerTextColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> rowBgColor1;
    extern std::unique_ptr<Engine::Graphics::RGBA> rowBgColor2;
    extern std::unique_ptr<Engine::Graphics::RGBA> rowTextColor;
    
    // Team colors for TEAM_DEATHMATCH mode
    extern std::unique_ptr<Engine::Graphics::RGBA> teamRedColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamBlueColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamGreenColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamYellowColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamPurpleColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamOrangeColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamCyanColor;
    extern std::unique_ptr<Engine::Graphics::RGBA> teamPinkColor;
    
    // Initialize all global colors
    void InitializeColors();
    
    // Get team color by index
    Engine::Graphics::RGBA* GetTeamColor(int teamIndex);
}
