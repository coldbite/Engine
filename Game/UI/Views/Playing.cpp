#include "Playing.h"
#include "../../../Engine/Graphics/IRenderingAPI.h"
#include <iostream>

Engine::Graphics::Text text;

Playing::Playing() : Engine::View("Playing") {
    text.LoadFont("C:/Windows/Fonts/arial.ttf", 22);
}

void Playing::OnShow() {
    std::cout << "[Playing] View shown" << std::endl;
}

void Playing::OnHide() {
    std::cout << "[Playing] View hidden" << std::endl;
}

void Playing::OnUpdate(float deltaTime) {
    if(IsActive()) {
        UpdateInternal(deltaTime);
    }
}


void Playing::Render(Engine::Graphics::IRenderingAPI& context) {
    context.Clear(0.0f, 0.0f, 0.0f, 1.0f);

    // Text rendering using IRenderingAPI abstraction
    text.RenderText(context, "Hello World!", 100, 100, 1.0f, text_color);
}

