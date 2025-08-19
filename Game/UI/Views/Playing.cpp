#include "Playing.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Engine::Graphics::Text text;

Playing::Playing() : Engine::View("Playing") {
    SetBackground(color_background);
    text.LoadFont("C:/Users/Bizzi/Documents/GitHub/Engine/Game/Assets/Fonts/Sansation-Regular.ttf", 18);
}

void Playing::OnShow() {
    std::cout << "[Playing] View shown" << std::endl;
}

void Playing::OnHide() {
    std::cout << "[Playing] View hidden" << std::endl;
}

void Playing::OnResize(int witdth, int height, int oldWidth, int oldHeight) {

}

void Playing::OnUpdate(float deltaTime) {
    // Currently no internal updates needed for Playing view
    // UpdateInternal(deltaTime) would be called here if we had animations or game logic
}

void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(GetBackground());

    text.RenderText(context, "Playing game...!", 100, 100, text_color);
}

