#pragma once
#include "../../../../Engine/Graphics/UI/Text/Text.h"

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Options {
public:
    Options();
    
    void Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void Update(float deltaTime);

private:
    Engine::Graphics::Text titleText;
    Engine::Graphics::Text optionsText;
};