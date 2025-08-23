#include "Menu.h"
#include "../../IRenderingAPI.h"

namespace Engine {
    namespace Graphics {

        Menu::Menu() : backgroundColor(new RGBA(128, 128, 0, 200)), // Olive/Yellow background
                       buttonSpacing(5.0f), buttonHeight(40.0f), keyCallback(nullptr) {
        }

        Menu::~Menu() {
            ClearButtons();
        }

        void Menu::Update(float deltaTime) {
            for (Button* button : buttons) {
                if (button) {
                    button->Update(deltaTime);
                }
            }
        }

        void Menu::Render(IRenderingAPI& context, float x, float y, float width, float height) {
            // Calculate total height needed for all buttons with equal spacing
            float totalButtonHeight = buttons.size() * buttonHeight + (buttons.size() + 1) * buttonSpacing;

            // Draw background (yellow area) from BOTTOM_LEFT upwards
            float backgroundHeight = totalButtonHeight;
            float backgroundY = y + height - backgroundHeight;

            context.DrawRect(x, backgroundY, width, backgroundHeight, backgroundColor);

            // Render buttons from bottom to top with equal spacing
            float currentY = y + height - buttonHeight - buttonSpacing; // Start from bottom with equal padding

            for (size_t i = 0; i < buttons.size(); i++) {
                Button* button = buttons[i];
                if (button) {
                    button->SetBounds(x, currentY, width, buttonHeight); // Full width, no margin
                    button->Render(context, x, currentY);
                    currentY -= (buttonHeight + buttonSpacing); // Move up for next button
                }
            }
        }

        void Menu::AddButton(const std::string& text, const std::string& key) {
            Button* button = new Button();
            button->SetValue(text);
            button->SetFont("Sansation");
            button->SetSize(20.0f);
            button->SetNormalColor(new RGBA(255, 255, 255, 100));
            button->SetNormalBackgroundColor(new RGBA(255, 0, 0, 200));
            button->SetPadding(0.0f, 0.0f);
            button->SetMargin(0.0f, 0.0f);
            button->SetStyle(Engine::Graphics::FontStyle::UPPERCASE);

            // Set click handler to trigger key callback
            button->SetOnClick([this, key]() {
                TriggerKey(key);
            });

            buttons.push_back(button);
        }

        void Menu::ClearButtons() {
            for (Button* button : buttons) {
                delete button;
            }
            buttons.clear();
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

        void Menu::TriggerKey(const std::string& key) {
            if (keyCallback) {
                keyCallback(key);
            }
        }
    }
}
