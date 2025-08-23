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
            void SetNormalColor(IColor* color);
            void SetHoverColor(IColor* color);
            void SetPressedColor(IColor* color);
            void SetDisabledColor(IColor* color);

            void SetNormalBackgroundColor(IColor* color);
            void SetHoverBackgroundColor(IColor* color);
            void SetPressedBackgroundColor(IColor* color);
            void SetDisabledBackgroundColor(IColor* color);

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
            IColor* m_normalTextColor;
            IColor* m_hoverTextColor;
            IColor* m_pressedTextColor;
            IColor* m_disabledTextColor;

            IColor* m_normalBackgroundColor;
            IColor* m_hoverBackgroundColor;
            IColor* m_pressedBackgroundColor;
            IColor* m_disabledBackgroundColor;

            // Store original colors
            IColor* m_originalTextColor;
            IColor* m_originalBackgroundColor;
        };
    }
}
