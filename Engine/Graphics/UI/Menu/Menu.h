#pragma once

#include "../Button/Button.h"
#include "../../RGBA.h"
#include <vector>
#include <functional>
#include <string>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
        
        class Menu {
        public:
            Menu();
            ~Menu();
            
            void Update(float deltaTime);
            void Render(IRenderingAPI& context, float x, float y, float width, float height);
            
            // Button management
            void AddButton(const std::string& text, const std::string& key);
            void ClearButtons();
            
            // Callback system
            void OnKey(std::function<void(const std::string&)> callback);
            
            // Appearance
            void SetBackgroundColor(IColor* color);
            void SetButtonSpacing(float spacing);
            
        private:
            void TriggerKey(const std::string& key);
            
            std::vector<Button*> buttons;
            IColor* backgroundColor;
            float buttonSpacing;
            float buttonHeight;
            std::function<void(const std::string&)> keyCallback;
        };
    }
}