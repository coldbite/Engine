#pragma once
#include "../../../../Engine/Graphics/UI/Text/Text.h"
#include "../../../../Engine/Graphics/UI/Table/Table.h"
#include <vector>
#include <memory>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Score {
public:
    enum GameMode {
        SINGLEPLAYER,
        TEAM_DEATHMATCH
    };

    Score();

    void Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void Update(float deltaTime);

    void SetGameMode(GameMode mode, int teams);
    GameMode GetGameMode() const { return gameMode; }

    // Table management for different modes
    void SetupSingleplayerTable();
    void SetupTeamDeathmatchTables(int teamCount = 2);

private:
    GameMode gameMode;

    // Single table for SINGLEPLAYER mode
    std::unique_ptr<Table> singleTable;

    // Multiple tables for TEAM_DEATHMATCH mode
    std::vector<std::unique_ptr<Table>> teamTables;
    std::vector<std::string> teamNames;

    // Reusable alignment vector to prevent recreation
    std::vector<HorizontalAlignment> alignments = {
        HorizontalAlignment::CENTER,
        HorizontalAlignment::LEFT,
        HorizontalAlignment::CENTER,
        HorizontalAlignment::CENTER,
        HorizontalAlignment::CENTER,
        HorizontalAlignment::RIGHT
    };
    
    // Colors are now globally defined in Game/Options.h

    void InitializeDefaultStyles();
    void RenderSingleplayerMode(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void RenderTeamDeathmatchMode(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void PopulateSampleData();
};
