#pragma once

#include "KeyboardInput.h"
#include "MouseInput.h"

namespace Engine {
    namespace Input {

        class InputManager {
        public:
            static InputManager& GetInstance();

            // Keyboard input delegation (for backward compatibility)
            void BindKey(KEY key, KeyCallback callback);
            void BindKey(KEY key, KeyAction action, KeyCallback callback);
            void UnbindKey(KEY key);
            void UnbindKey(KEY key, KeyAction action);
            bool IsKeyPressed(KEY key) const;
            bool IsKeyDown(KEY key) const;
            bool IsKeyReleased(KEY key) const;

            // Mouse input delegation
            void BindMouseButton(MouseButton button, MouseButtonCallback callback);
            void BindMouseButton(MouseButton button, MouseAction action, MouseButtonCallback callback);
            void UnbindMouseButton(MouseButton button);
            void BindMouseMove(MouseMoveCallback callback);
            void UnbindMouseMove();
            void BindMouseScroll(MouseScrollCallback callback);
            void UnbindMouseScroll();
            bool IsMouseButtonPressed(MouseButton button) const;
            bool IsMouseButtonDown(MouseButton button) const;
            bool IsMouseButtonReleased(MouseButton button) const;
            MousePosition GetMousePosition() const;
            MouseDelta GetMouseDelta() const;

            // Input system access
            KeyboardInput& GetKeyboard() { return KeyboardInput::GetInstance(); }
            MouseInput& GetMouse() { return MouseInput::GetInstance(); }
            const KeyboardInput& GetKeyboard() const { return KeyboardInput::GetInstance(); }
            const MouseInput& GetMouse() const { return MouseInput::GetInstance(); }

            // Internal event handling (called by NativeWindow)
            void OnKeyEvent(int platformKey, int platformAction);
            void OnMouseButtonEvent(int platformButton, int platformAction, float x, float y);
            void OnMouseMoveEvent(float x, float y);
            void OnMouseScrollEvent(float deltaX, float deltaY);

            // Update method (call once per frame)
            void Update();

            // Clear all bindings
            void ClearBindings();

        private:
            InputManager() = default;
            ~InputManager() = default;
            InputManager(const InputManager&) = delete;
            InputManager& operator=(const InputManager&) = delete;
        };

        // Convenience function for global access
        inline InputManager& GetInput() { return InputManager::GetInstance(); }
    }
}