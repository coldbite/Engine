#pragma once

#include "../Text/Text.h"
#include <functional>

namespace Engine {
    namespace Graphics {
        
        enum class ButtonState {
            NORMAL,
            HOVERED,
            PRESSED,
            DISABLED
        };

        class Button : public Text {
        public:
            Button();
            ~Button();

            // Event callbacks
            void SetOnClick(std::function<void()> callback);
            void SetOnHover(std::function<void()> callback);
            void SetOnHoverEnd(std::function<void()> callback);
            void SetOnPress(std::function<void()> callback);
            void SetOnRelease(std::function<void()> callback);

            // Button state management
            void SetState(ButtonState state);
            ButtonState GetState() const { return m_state; }
            
            // Enable/disable button
            void SetEnabled(bool enabled);
            bool IsEnabled() const { return m_enabled; }

            // Button appearance for different states
            void SetNormalColor(const IColor& color);
            void SetHoverColor(const IColor& color);
            void SetPressedColor(const IColor& color);
            void SetDisabledColor(const IColor& color);

            void SetNormalBackgroundColor(const IColor& color);
            void SetHoverBackgroundColor(const IColor& color);
            void SetPressedBackgroundColor(const IColor& color);
            void SetDisabledBackgroundColor(const IColor& color);

            // Mouse interaction (to be called from input system)
            void OnMouseMove(float x, float y);
            void OnMouseDown(float x, float y);
            void OnMouseUp(float x, float y);
            void OnMouseEnter();
            void OnMouseLeave();

            // Button bounds for hit testing
            void SetBounds(float x, float y, float width, float height);
            bool IsPointInside(float x, float y) const;

            // Override render to apply current state colors
            void Render(IRenderingAPI& context, float x, float y) override;
            void Render(IRenderingAPI& context, const TextAlignment& alignment) override;

            // Update method for animations and state transitions
            void Update(float deltaTime) override;

        private:
            void UpdateAppearanceForState();
            void TriggerOnClick();
            void TriggerOnHover();
            void TriggerOnHoverEnd();
            void TriggerOnPress();
            void TriggerOnRelease();

            // Button state
            ButtonState m_state;
            bool m_enabled;
            bool m_isMouseInside;
            bool m_isPressed;

            // Button bounds
            float m_x, m_y, m_width, m_height;

            // Event callbacks
            std::function<void()> m_onClickCallback;
            std::function<void()> m_onHoverCallback;
            std::function<void()> m_onHoverEndCallback;
            std::function<void()> m_onPressCallback;
            std::function<void()> m_onReleaseCallback;

            // State-specific colors
            RGBA m_normalTextColor;
            RGBA m_hoverTextColor;
            RGBA m_pressedTextColor;
            RGBA m_disabledTextColor;

            RGBA m_normalBackgroundColor;
            RGBA m_hoverBackgroundColor;
            RGBA m_pressedBackgroundColor;
            RGBA m_disabledBackgroundColor;

            // Store original colors
            RGBA m_originalTextColor;
            RGBA m_originalBackgroundColor;
        };
    }
}