#include "Options.h"

namespace UIColors {
    // Score table colors - globally allocated once
    std::unique_ptr<Engine::Graphics::RGBA> headerBgColor;
    std::unique_ptr<Engine::Graphics::RGBA> headerTextColor;
    std::unique_ptr<Engine::Graphics::RGBA> rowBgColor1;
    std::unique_ptr<Engine::Graphics::RGBA> rowBgColor2;
    std::unique_ptr<Engine::Graphics::RGBA> rowTextColor;
    
    // Team colors for TEAM_DEATHMATCH mode
    std::unique_ptr<Engine::Graphics::RGBA> teamRedColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamBlueColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamGreenColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamYellowColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamPurpleColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamOrangeColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamCyanColor;
    std::unique_ptr<Engine::Graphics::RGBA> teamPinkColor;
    
    void InitializeColors() {
        // Initialize score table colors once
        headerBgColor = std::make_unique<Engine::Graphics::RGBA>(0, 0, 0, 0);
        headerTextColor = std::make_unique<Engine::Graphics::RGBA>(255, 255, 255, 100);
        rowBgColor1 = std::make_unique<Engine::Graphics::RGBA>(0, 0, 0, 30);
        rowBgColor2 = std::make_unique<Engine::Graphics::RGBA>(0, 0, 0, 50);
        rowTextColor = std::make_unique<Engine::Graphics::RGBA>(220, 220, 220, 100);
        
        // Initialize team colors once
        teamRedColor = std::make_unique<Engine::Graphics::RGBA>(120, 50, 50, 80);
        teamBlueColor = std::make_unique<Engine::Graphics::RGBA>(50, 50, 120, 80);
        teamGreenColor = std::make_unique<Engine::Graphics::RGBA>(50, 120, 50, 80);
        teamYellowColor = std::make_unique<Engine::Graphics::RGBA>(120, 120, 50, 80);
        teamPurpleColor = std::make_unique<Engine::Graphics::RGBA>(100, 50, 120, 80);
        teamOrangeColor = std::make_unique<Engine::Graphics::RGBA>(120, 80, 50, 80);
        teamCyanColor = std::make_unique<Engine::Graphics::RGBA>(50, 120, 120, 80);
        teamPinkColor = std::make_unique<Engine::Graphics::RGBA>(120, 80, 100, 80);
    }
    
    Engine::Graphics::RGBA* GetTeamColor(int teamIndex) {
        switch (teamIndex) {
            case 0: return teamRedColor.get();
            case 1: return teamBlueColor.get();
            case 2: return teamGreenColor.get();
            case 3: return teamYellowColor.get();
            case 4: return teamPurpleColor.get();
            case 5: return teamOrangeColor.get();
            case 6: return teamCyanColor.get();
            case 7: return teamPinkColor.get();
            default: return teamRedColor.get(); // Fallback to red
        }
    }
}