#pragma once

#include "../Button/Button.h"
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
            
            // Mouse event handling
            void OnMouseMove(float x, float y);
            void OnMouseDown(float x, float y);
            void OnMouseUp(float x, float y);
            
            // Button management
            void AddButton(const std::string& text, const std::string& key);
            void AddSpace(int height);
            void ClearButtons();
            
            // Callback system
            void OnKey(std::function<void(const std::string&)> callback);
            
            // Appearance
            void SetBackgroundColor(IColor* color);
            void SetButtonSpacing(float spacing);
            
        private:
            struct MenuItem {
                enum Type { BUTTON, SPACE };
                Type type;
                Button* button;
                int spaceHeight;
                
                MenuItem(Button* btn) : type(BUTTON), button(btn), spaceHeight(0) {}
                MenuItem(int height) : type(SPACE), button(nullptr), spaceHeight(height) {}
            };
            
            void TriggerKey(const std::string& key);
            
            std::vector<MenuItem> menuItems;
            IColor* backgroundColor;
            float buttonSpacing;
            std::function<void(const std::string&)> keyCallback;
        };
    }
}
