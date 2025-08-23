#include "ServerInfo.h"
#include "../../../../Engine/Graphics/IRenderingAPI.h"

ServerInfo::ServerInfo() {
    titleText.SetValue("SERVER INFO");
    titleText.SetFont("Sansation");
    titleText.SetSize(28.0f);
    titleText.SetColor(new Engine::Graphics::RGBA(255, 255, 255, 255));
    titleText.SetStyle(Engine::Graphics::FontStyle::BOLD | Engine::Graphics::FontStyle::UPPERCASE);
    titleText.SetPadding(10.0f, 10.0f);

    serverText.SetValue("Server: Game Server #1\nStatus: Online\nPlayers: 24/32\nPing: 45ms\nUptime: 2d 14h 23m\n\nServer Region: Europe\nGame Mode: Standard");
    serverText.SetFont("Sansation");
    serverText.SetSize(18.0f);
    serverText.SetColor(new Engine::Graphics::RGBA(220, 220, 220, 255));
    serverText.SetPadding(15.0f, 10.0f);
}

void ServerInfo::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    titleText.Render(context, x, y);
    serverText.Render(context, x, y + 60.0f);
}

void ServerInfo::Update(float deltaTime) {
    titleText.Update(deltaTime);
    serverText.Update(deltaTime);
}