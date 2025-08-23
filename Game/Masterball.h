#pragma once
#include "../Engine/Core/Game.h"
#include "Options.h"
#include "Map.h"
#include "UI/Views/Shutdown.h"
#include "UI/Views/Loading.h"
#include "UI/Views/Playing.h"

class Masterball : public Engine::Game {
public:
    void OnInit() override;
    void OnShutdown() override;

private:
    Map map;
    std::shared_ptr<::Shutdown> shutdown;
    std::shared_ptr<Loading> loading;
    std::shared_ptr<Playing> playing;

    void InitializeViews();
};
