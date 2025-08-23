#include "Button.h"
#include "../../IRenderingAPI.h"

namespace Engine {
    namespace Graphics {

        Button::Button()
            : Text(),
              m_state(ButtonState::NORMAL),
              m_enabled(true),
              m_isMouseInside(false),
              m_isPressed(false),
              m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f),
              m_onClickCallback(nullptr),
              m_onHoverCallback(nullptr),
              m_onHoverEndCallback(nullptr),
              m_onPressCallback(nullptr),
              m_onReleaseCallback(nullptr),
              m_normalTextColor(new RGBA(255, 255, 255, 100)),
              m_hoverTextColor(new RGBA(199, 199, 255, 100)),
              m_pressedTextColor(new RGBA(150, 150, 199, 100)),
              m_disabledTextColor(new RGBA(127, 127, 127, 100)),
              m_normalBackgroundColor(new RGBA(0, 0, 0, 0)),
              m_hoverBackgroundColor(new RGBA(50, 50, 50, 39)),
              m_pressedBackgroundColor(new RGBA(99, 99, 99, 59)),
              m_disabledBackgroundColor(new RGBA(30, 30, 30, 20)),
              m_originalTextColor(new RGBA(255, 255, 255, 100)),
              m_originalBackgroundColor(new RGBA(0, 0, 0, 0))
        {
            // Colors are now initialized in the constructor initializer list
        }

        void Button::SetOnClick(std::function<void()> callback) {
            m_onClickCallback = callback;
        }

        void Button::SetOnHover(std::function<void()> callback) {
            m_onHoverCallback = callback;
        }

        void Button::SetOnHoverEnd(std::function<void()> callback) {
            m_onHoverEndCallback = callback;
        }

        void Button::SetOnPress(std::function<void()> callback) {
            m_onPressCallback = callback;
        }

        void Button::SetOnRelease(std::function<void()> callback) {
            m_onReleaseCallback = callback;
        }

        void Button::SetState(ButtonState state) {
            if (m_state != state) {
                m_state = state;
                UpdateAppearanceForState();
            }
        }

        void Button::SetEnabled(bool enabled) {
            m_enabled = enabled;
            if (!enabled) {
                SetState(ButtonState::DISABLED);
            } else if (m_state == ButtonState::DISABLED) {
                SetState(ButtonState::NORMAL);
            }
        }

        void Button::SetNormalColor(IColor* color) {
            m_normalTextColor = color;
        }

        void Button::SetHoverColor(IColor* color) {
            m_hoverTextColor = color;
        }

        void Button::SetPressedColor(IColor* color) {
            m_pressedTextColor = color;
        }

        void Button::SetDisabledColor(IColor* color) {
            m_disabledTextColor = color;
        }

        void Button::SetNormalBackgroundColor(IColor* color) {
            m_normalBackgroundColor = color;
        }

        void Button::SetHoverBackgroundColor(IColor* color) {
            m_hoverBackgroundColor = color;
        }

        void Button::SetPressedBackgroundColor(IColor* color) {
            m_pressedBackgroundColor = color;
        }

        void Button::SetDisabledBackgroundColor(IColor* color) {
            m_disabledBackgroundColor = color;
        }

        void Button::OnMouseMove(float x, float y) {
            if (!m_enabled) return;

            bool wasInside = m_isMouseInside;
            m_isMouseInside = IsPointInside(x, y);

            if (m_isMouseInside && !wasInside) {
                OnMouseEnter();
            } else if (!m_isMouseInside && wasInside) {
                OnMouseLeave();
            }
        }

        void Button::OnMouseDown(float x, float y) {
            if (!m_enabled || !IsPointInside(x, y)) return;

            m_isPressed = true;
            SetState(ButtonState::PRESSED);
            TriggerOnPress();
        }

        void Button::OnMouseUp(float x, float y) {
            if (!m_enabled) return;

            bool wasPressed = m_isPressed;
            m_isPressed = false;

            if (IsPointInside(x, y)) {
                SetState(ButtonState::HOVERED);
                if (wasPressed) {
                    TriggerOnClick();
                }
            } else {
                SetState(ButtonState::NORMAL);
            }

            if (wasPressed) {
                TriggerOnRelease();
            }
        }

        void Button::OnMouseEnter() {
            if (!m_enabled) return;

            m_isMouseInside = true;
            if (!m_isPressed) {
                SetState(ButtonState::HOVERED);
            }
            TriggerOnHover();
        }

        void Button::OnMouseLeave() {
            if (!m_enabled) return;

            m_isMouseInside = false;
            if (!m_isPressed) {
                SetState(ButtonState::NORMAL);
            }
            TriggerOnHoverEnd();
        }

        void Button::SetBounds(float x, float y, float width, float height) {
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;
        }

        bool Button::IsPointInside(float x, float y) const {
            return (x >= m_x && x <= m_x + m_width &&
                    y >= m_y && y <= m_y + m_height);
        }

        void Button::Render(IRenderingAPI& context, float x, float y) {
            UpdateAppearanceForState();
            
            // Draw button background using full button bounds instead of just text bounds
            IColor* currentBgColor = nullptr;
            switch (m_state) {
                case ButtonState::NORMAL:
                    currentBgColor = m_normalBackgroundColor;
                    break;
                case ButtonState::HOVERED:
                    currentBgColor = m_hoverBackgroundColor;
                    break;
                case ButtonState::PRESSED:
                    currentBgColor = m_pressedBackgroundColor;
                    break;
                case ButtonState::DISABLED:
                    currentBgColor = m_disabledBackgroundColor;
                    break;
            }
            
            if (currentBgColor && currentBgColor->GetAlpha() > 0) {
                context.DrawRect(m_x, m_y, m_width, m_height, currentBgColor);
                
                // Temporarily disable text background to prevent overlapping transparency
                SetBackgroundEnabled(false);
            }
            
            // Center the text within the button bounds
            Padding p = GetPadding();
            float textX = m_x + p.left;
            float textY = m_y + p.top + (m_height - p.top - p.bottom - GetTextHeight()) / 2.0f;
            
            // Render text centered
            Text::Render(context, textX, textY);
            
            // Re-enable text background if we disabled it
            if (currentBgColor && currentBgColor->GetAlpha() > 0) {
                SetBackgroundEnabled(true);
            }
        }

        void Button::Render(IRenderingAPI& context, const TextAlignment& alignment) {
            UpdateAppearanceForState();
            Text::Render(context, alignment);
        }

        void Button::Update(float deltaTime) {
            Text::Update(deltaTime);
            // Add any button-specific update logic here
        }

        void Button::UpdateAppearanceForState() {
            switch (m_state) {
                case ButtonState::NORMAL:
                    SetColor(m_normalTextColor);
                    SetBackground(m_normalBackgroundColor);
                    break;
                case ButtonState::HOVERED:
                    SetColor(m_hoverTextColor);
                    SetBackground(m_hoverBackgroundColor);
                    break;
                case ButtonState::PRESSED:
                    SetColor(m_pressedTextColor);
                    SetBackground(m_pressedBackgroundColor);
                    break;
                case ButtonState::DISABLED:
                    SetColor(m_disabledTextColor);
                    SetBackground(m_disabledBackgroundColor);
                    break;
            }
        }

        void Button::TriggerOnClick() {
            if (m_onClickCallback) {
                m_onClickCallback();
            }
        }

        void Button::TriggerOnHover() {
            if (m_onHoverCallback) {
                m_onHoverCallback();
            }
        }

        void Button::TriggerOnHoverEnd() {
            if (m_onHoverEndCallback) {
                m_onHoverEndCallback();
            }
        }

        void Button::TriggerOnPress() {
            if (m_onPressCallback) {
                m_onPressCallback();
            }
        }

        void Button::TriggerOnRelease() {
            if (m_onReleaseCallback) {
                m_onReleaseCallback();
            }
        }

    }
}
