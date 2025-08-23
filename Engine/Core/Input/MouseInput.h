#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace Engine {
    namespace Input {

        enum class MouseButton {
            LEFT = 0,
            RIGHT = 1,
            MIDDLE = 2
        };

        enum class MouseAction {
            PRESS,
            RELEASE
        };

        struct MousePosition {
            float x = 0.0f;
            float y = 0.0f;
        };

        struct MouseDelta {
            float deltaX = 0.0f;
            float deltaY = 0.0f;
        };

        // Callback types
        using MouseButtonCallback = std::function<void(MouseButton button, MouseAction action, float x, float y)>;
        using MouseMoveCallback = std::function<void(float x, float y, float deltaX, float deltaY)>;
        using MouseScrollCallback = std::function<void(float deltaX, float deltaY)>;

        class MouseInput {
        public:
            static MouseInput& GetInstance();

            // Mouse button events
            void BindMouseButton(MouseButton button, MouseButtonCallback callback);
            void BindMouseButton(MouseButton button, MouseAction action, MouseButtonCallback callback);
            void UnbindMouseButton(MouseButton button);
            void UnbindMouseButton(MouseButton button, MouseAction action);

            // Mouse movement events
            void BindMouseMove(MouseMoveCallback callback);
            void UnbindMouseMove();

            // Mouse scroll events
            void BindMouseScroll(MouseScrollCallback callback);
            void UnbindMouseScroll();

            // Mouse state queries
            bool IsButtonPressed(MouseButton button) const;  // Just pressed this frame
            bool IsButtonDown(MouseButton button) const;     // Currently held down
            bool IsButtonReleased(MouseButton button) const; // Just released this frame

            MousePosition GetPosition() const { return currentPosition; }
            MouseDelta GetDelta() const { return mouseDelta; }

            // Internal event handling (called by InputManager/NativeWindow)
            void OnMouseButtonEvent(int platformButton, int platformAction, float x, float y);
            void OnMouseMoveEvent(float x, float y);
            void OnMouseScrollEvent(float deltaX, float deltaY);
            
            // Update method (call once per frame)
            void Update();

            // Clear all bindings
            void ClearBindings();

        private:
            MouseInput() = default;
            ~MouseInput() = default;
            MouseInput(const MouseInput&) = delete;
            MouseInput& operator=(const MouseInput&) = delete;

            // Convert platform mouse button to our enum
            MouseButton ConvertPlatformButton(int platformButton) const;
            MouseAction ConvertPlatformAction(int platformAction) const;

            // Mouse button bindings
            struct MouseButtonBinding {
                MouseAction action = MouseAction::PRESS;
                MouseButtonCallback callback;
            };

            std::unordered_map<MouseButton, std::vector<MouseButtonBinding>> buttonBindings;

            // Mouse movement and scroll bindings
            MouseMoveCallback moveCallback;
            MouseScrollCallback scrollCallback;

            // Mouse state tracking
            std::unordered_map<MouseButton, bool> currentButtonStates;
            std::unordered_map<MouseButton, bool> previousButtonStates;

            MousePosition currentPosition;
            MousePosition previousPosition;
            MouseDelta mouseDelta;
        };

        // Convenience function for global access
        inline MouseInput& GetMouse() { return MouseInput::GetInstance(); }
    }
}