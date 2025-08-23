#include "Menu.h"
#include "../../IRenderingAPI.h"
#include <iostream>
#include <ostream>

namespace Engine {
    namespace Graphics {

        Menu::Menu() : backgroundColor(new RGBA(0, 0, 0, 0)), // Olive/Yellow background
                       buttonSpacing(2.0f), keyCallback(nullptr) {
        }

        Menu::~Menu() {
            ClearButtons();
        }

        void Menu::Update(float deltaTime) {
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    item.button->Update(deltaTime);
                }
            }
        }

        void Menu::Render(IRenderingAPI& context, float x, float y, float width, float height) {
            size_t item_count = menuItems.size();
            float totalHeight = 0.0f;

            if(item_count == 0) {
                return;
            }

            // Hintergrundhöhe berechnen
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    float btnHeight = item.button->GetTextHeight();
                    Padding p = item.button->GetPadding();
                    btnHeight += p.top + p.bottom;
                    totalHeight += btnHeight;
                } else if(item.type == MenuItem::SPACE) {
                    totalHeight += item.spaceHeight;
                }
            }

            // Add spacing between items
            if(item_count > 1) {
                totalHeight += (item_count - 1) * buttonSpacing;
            }

            float backgroundY = y + height - totalHeight;

            // Hintergrund zeichnen
            context.DrawRect(x, backgroundY, width, totalHeight, backgroundColor);

            // Items rendern (von oben nach unten)
            float currentY = backgroundY;

            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    float btnHeight = item.button->GetTextHeight();
                    Padding p = item.button->GetPadding();
                    btnHeight += p.top + p.bottom;

                    item.button->SetBounds(x, currentY, width, btnHeight);
                    item.button->Render(context, x, currentY);
                    
                    currentY += btnHeight + buttonSpacing;
                } else if(item.type == MenuItem::SPACE) {
                    currentY += item.spaceHeight + buttonSpacing;
                }
            }
        }

        void Menu::AddButton(const std::string& text, const std::string& key) {
            Button* button = new Button();
            button->SetValue(text);
            button->SetFont("Sansation");
            button->SetSize(20.0f);
            button->SetNormalColor(new RGBA(255, 255, 255, 100));
            button->SetNormalBackgroundColor(new RGBA(0, 0, 0, 40));
            button->SetHoverBackgroundColor(new RGBA(0, 0, 0, 20));
            button->SetPadding(5.0f, 5.0f);
            button->SetMargin(0.0f, 0.0f);
            //button->SetStyle(Engine::Graphics::FontStyle::UPPERCASE);

            // Set click handler to trigger key callback
            button->SetOnClick([this, key]() {
                TriggerKey(key);
                std::cout << "ONCLICK: " + key << std::endl;
            });

            menuItems.push_back(MenuItem(button));
        }

        void Menu::AddSpace(int height) {
            menuItems.push_back(MenuItem(height));
        }

        void Menu::ClearButtons() {
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    delete item.button;
                }
            }

            menuItems.clear();
        }

        void Menu::OnKey(std::function<void(const std::string&)> callback) {
            keyCallback = callback;
        }

        void Menu::SetBackgroundColor(IColor* color) {
            backgroundColor = color;
        }

        void Menu::SetButtonSpacing(float spacing) {
            buttonSpacing = spacing;
        }

        void Menu::OnMouseMove(float x, float y) {
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    item.button->OnMouseMove(x, y);
                }
            }
        }

        void Menu::OnMouseDown(float x, float y) {
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    item.button->OnMouseDown(x, y);
                }
            }
        }

        void Menu::OnMouseUp(float x, float y) {
            for(const MenuItem& item : menuItems) {
                if(item.type == MenuItem::BUTTON && item.button) {
                    item.button->OnMouseUp(x, y);
                }
            }
        }

        void Menu::TriggerKey(const std::string& key) {
            if(keyCallback) {
                keyCallback(key);
            }
        }
    }
}
