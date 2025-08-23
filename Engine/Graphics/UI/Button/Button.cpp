#include "Button.h"

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
              m_normalTextColor(1.0f, 1.0f, 1.0f, 1.0f),
              m_hoverTextColor(0.78f, 0.78f, 1.0f, 1.0f),
              m_pressedTextColor(0.59f, 0.59f, 0.78f, 1.0f),
              m_disabledTextColor(0.5f, 0.5f, 0.5f, 1.0f),
              m_normalBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f),
              m_hoverBackgroundColor(0.196f, 0.196f, 0.196f, 0.39f),
              m_pressedBackgroundColor(0.39f, 0.39f, 0.39f, 0.59f),
              m_disabledBackgroundColor(0.118f, 0.118f, 0.118f, 0.196f),
              m_originalTextColor(1.0f, 1.0f, 1.0f, 1.0f),
              m_originalBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f)
        {
            // Colors are now initialized in the constructor initializer list
        }

        Button::~Button() {
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

        void Button::SetNormalColor(const IColor& color) {
            m_normalTextColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetHoverColor(const IColor& color) {
            m_hoverTextColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetPressedColor(const IColor& color) {
            m_pressedTextColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetDisabledColor(const IColor& color) {
            m_disabledTextColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetNormalBackgroundColor(const IColor& color) {
            m_normalBackgroundColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetHoverBackgroundColor(const IColor& color) {
            m_hoverBackgroundColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetPressedBackgroundColor(const IColor& color) {
            m_pressedBackgroundColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Button::SetDisabledBackgroundColor(const IColor& color) {
            m_disabledBackgroundColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
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
            Text::Render(context, x, y);
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