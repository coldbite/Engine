#include "Score.h"
#include "../../../../Engine/Graphics/IRenderingAPI.h"
#include "../../../../Engine/Graphics/RGBA.h"
#include "../../../Options.h"
#include <algorithm>

Score::Score() : gameMode(SINGLEPLAYER) {
    // Initialize global colors if not already done
    if (!UIColors::headerBgColor) {
        UIColors::InitializeColors();
    }
    
    SetGameMode(TEAM_DEATHMATCH, 4);
}

void Score::SetGameMode(GameMode mode, int teams) {
    gameMode = mode;

    if (gameMode == SINGLEPLAYER) {
        SetupSingleplayerTable();
    } else if (gameMode == TEAM_DEATHMATCH) {
        SetupTeamDeathmatchTables(teams); // Default 4 teams
    }
}

void Score::SetupSingleplayerTable() {
    singleTable = std::make_unique<Table>();

    // Customize table appearance using global colors
    singleTable->SetStyle(Table::HEADER_BACKGROUND, UIColors::headerBgColor.get());
    singleTable->SetStyle(Table::HEADER_TEXT_COLOR, UIColors::headerTextColor.get());
    singleTable->SetStyle(Table::HEADER_TEXT_STYLE, Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
    singleTable->SetStyle(Table::ROW_BACKGROUND, UIColors::rowBgColor1.get());
    singleTable->SetStyle(Table::ROW_ALT_BACKGROUND, UIColors::rowBgColor2.get());
    singleTable->SetStyle(Table::ROW_TEXT_COLOR, UIColors::rowTextColor.get());
    singleTable->SetStyle(Table::ROW_TEXT_STYLE, Engine::Graphics::FontStyle::NORMAL);

    singleTable->SetRowHeight(25.0f);
    singleTable->SetHeaderHeight(30.0f);

    // Add headers with colspan for team name
    singleTable->AddHeader("Team 1", 3, alignments[0]); // Spans first 3 columns (Index, PlayerName, Rank)
    singleTable->AddHeader("K", 1, alignments[3]);
    singleTable->AddHeader("D", 1, alignments[4]);
    singleTable->AddHeader("SCORE", 1, alignments[5]);

    // Add sample data
    singleTable->AddRow({"1", "Player1", "1", "24", "8", "2450"}, alignments);
    singleTable->AddRow({"2", "Player2", "2", "21", "12", "2120"}, alignments);
    singleTable->AddRow({"3", "Player3", "3", "18", "15", "1890"}, alignments);
    singleTable->AddRow({"4", "Player4", "4", "16", "18", "1670"}, alignments);
    singleTable->AddRow({"5", "Player5", "5", "14", "20", "1450"}, alignments);
    singleTable->AddRow({"6", "Player6", "6", "12", "22", "1230"}, alignments);
    singleTable->AddRow({"7", "Player7", "7", "11", "25", "1100"}, alignments);
    singleTable->AddRow({"8", "Player8", "8", "9", "27", "980"}, alignments);
}

void Score::SetupTeamDeathmatchTables(int teamCount) {
    teamTables.clear();
    teamNames.clear();

    teamCount = std::max(2, std::min(8, teamCount)); // Clamp between 2-8 teams

    std::vector<std::string> defaultTeamNames = {"Red Team", "Blue Team", "Green Team", "Yellow Team",
                                                "Purple Team", "Orange Team", "Cyan Team", "Pink Team"};

    // Use global team colors instead of creating new ones

    for (int i = 0; i < teamCount; ++i) {
        teamNames.push_back(defaultTeamNames[i]);

        auto table = std::make_unique<Table>();

        // Customize each team table with team-specific header color using global colors
        table->SetStyle(Table::HEADER_BACKGROUND, UIColors::GetTeamColor(i));
        table->SetStyle(Table::HEADER_TEXT_COLOR, UIColors::headerTextColor.get());
        table->SetStyle(Table::HEADER_TEXT_STYLE, Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
        table->SetStyle(Table::ROW_BACKGROUND, UIColors::rowBgColor2.get()); // 50
        table->SetStyle(Table::ROW_ALT_BACKGROUND, UIColors::rowBgColor1.get()); // 30
        table->SetStyle(Table::ROW_TEXT_COLOR, UIColors::rowTextColor.get());
        table->SetStyle(Table::ROW_TEXT_STYLE, Engine::Graphics::FontStyle::NORMAL);

        table->SetRowHeight(22.0f);
        table->SetHeaderHeight(25.0f);

        // Add headers with colspan for team name
        table->AddHeader(defaultTeamNames[i], 3); // Spans first 3 columns (Index, PlayerName, Rank)
        table->AddHeader("K");
        table->AddHeader("D");
        table->AddHeader("SCORE");

        // Add sample team data
        table->AddRow({std::to_string(i*3 + 1), "Player" + std::to_string(i*3 + 1), "1", "15", "7", std::to_string(1500 + i*200)}, alignments);
        table->AddRow({std::to_string(i*3 + 2), "Player" + std::to_string(i*3 + 2), "2", "12", "8", std::to_string(1200 + i*150)}, alignments);
        table->AddRow({std::to_string(i*3 + 3), "Player" + std::to_string(i*3 + 3), "3", "9", "11", std::to_string(900 + i*100)}, alignments);

        teamTables.push_back(std::move(table));
    }
}

void Score::Update(float deltaTime) {
    if (gameMode == SINGLEPLAYER && singleTable) {
        singleTable->Update(deltaTime);
    } else if (gameMode == TEAM_DEATHMATCH) {
        for (auto& table : teamTables) {
            table->Update(deltaTime);
        }
    }
}

void Score::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    float contentY = y + 50.0f;
    float contentHeight = height - 50.0f;

    if (gameMode == SINGLEPLAYER) {
        RenderSingleplayerMode(context, x, contentY, width, contentHeight);
    } else if (gameMode == TEAM_DEATHMATCH) {
        RenderTeamDeathmatchMode(context, x, contentY, width, contentHeight);
    }
}

void Score::RenderSingleplayerMode(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    if (singleTable) {
        singleTable->Render(context, x, y, width, height);
    }
}

void Score::RenderTeamDeathmatchMode(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    if (teamTables.empty()) return;

    int teamCount = static_cast<int>(teamTables.size());

    // Calculate grid layout
    int cols = (teamCount <= 2) ? teamCount : (teamCount <= 4) ? 2 : (teamCount <= 6) ? 3 : 4;
    int rows = (teamCount + cols - 1) / cols; // Ceiling division

    float tableWidth = (width - (cols - 1) * 10.0f) / cols; // 10px spacing between tables
    float tableHeight = (height - (rows - 1) * 10.0f) / rows; // 10px spacing between rows

    for (int i = 0; i < teamCount; ++i) {
        int row = i / cols;
        int col = i % cols;

        float tableX = x + col * (tableWidth + 10.0f);
        float tableY = y + row * (tableHeight + 10.0f);

        // Team names are already rendered as table headers - skip separate rendering
        // This was creating memory leaks by creating new Text objects every frame

        // Render the team table
        teamTables[i]->Render(context, tableX, tableY + 20.0f, tableWidth, tableHeight - 20.0f);
    }
}
