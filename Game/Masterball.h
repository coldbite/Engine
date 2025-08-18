#pragma once
#include "../Engine/Core/Game.h"
#include "Options.h"
#include "Map.h"
#include "UI/Views/LoadingView.h"
#include "UI/Views/PlayingView.h"

class Masterball : public Engine::Game {
public:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnShutdown() override;

private:
    Map map;
    std::shared_ptr<LoadingView> loadingView;
    std::shared_ptr<PlayingView> playingView;
    
    void InitializeViews();
};
