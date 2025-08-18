#pragma once
#include "../Engine/Core/Game.h"
#include "Options.h"
#include "Map.h"

class Masterball : public Engine::Game {
public:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnShutdown() override;

private:
    Map map;
};
