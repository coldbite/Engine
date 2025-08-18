#include "Masterball.h"

using Engine::Config;

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
