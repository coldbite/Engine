#include "InputManager.h"

namespace Engine {
    namespace Input {
        
        InputManager& InputManager::GetInstance() {
            static InputManager instance;
            return instance;
        }
        
        // Keyboard input delegation (for backward compatibility)
        void InputManager::BindKey(KEY key, KeyCallback callback) {
            GetKeyboard().BindKey(key, callback);
        }
        
        void InputManager::BindKey(KEY key, KeyAction action, KeyCallback callback) {
            GetKeyboard().BindKey(key, action, callback);
        }
        
        void InputManager::UnbindKey(KEY key) {
            GetKeyboard().UnbindKey(key);
        }
        
        void InputManager::UnbindKey(KEY key, KeyAction action) {
            GetKeyboard().UnbindKey(key, action);
        }
        
        bool InputManager::IsKeyPressed(KEY key) const {
            return GetKeyboard().IsKeyPressed(key);
        }
        
        bool InputManager::IsKeyDown(KEY key) const {
            return GetKeyboard().IsKeyDown(key);
        }
        
        bool InputManager::IsKeyReleased(KEY key) const {
            return GetKeyboard().IsKeyReleased(key);
        }

        // Mouse input delegation
        void InputManager::BindMouseButton(MouseButton button, MouseButtonCallback callback) {
            GetMouse().BindMouseButton(button, callback);
        }

        void InputManager::BindMouseButton(MouseButton button, MouseAction action, MouseButtonCallback callback) {
            GetMouse().BindMouseButton(button, action, callback);
        }

        void InputManager::UnbindMouseButton(MouseButton button) {
            GetMouse().UnbindMouseButton(button);
        }

        void InputManager::BindMouseMove(MouseMoveCallback callback) {
            GetMouse().BindMouseMove(callback);
        }

        void InputManager::UnbindMouseMove() {
            GetMouse().UnbindMouseMove();
        }

        void InputManager::BindMouseScroll(MouseScrollCallback callback) {
            GetMouse().BindMouseScroll(callback);
        }

        void InputManager::UnbindMouseScroll() {
            GetMouse().UnbindMouseScroll();
        }

        bool InputManager::IsMouseButtonPressed(MouseButton button) const {
            return GetMouse().IsButtonPressed(button);
        }

        bool InputManager::IsMouseButtonDown(MouseButton button) const {
            return GetMouse().IsButtonDown(button);
        }

        bool InputManager::IsMouseButtonReleased(MouseButton button) const {
            return GetMouse().IsButtonReleased(button);
        }

        MousePosition InputManager::GetMousePosition() const {
            return GetMouse().GetPosition();
        }

        MouseDelta InputManager::GetMouseDelta() const {
            return GetMouse().GetDelta();
        }
        
        // Internal event handling (called by NativeWindow)
        void InputManager::OnKeyEvent(int platformKey, int platformAction) {
            GetKeyboard().OnKeyEvent(platformKey, platformAction);
        }

        void InputManager::OnMouseButtonEvent(int platformButton, int platformAction, float x, float y) {
            GetMouse().OnMouseButtonEvent(platformButton, platformAction, x, y);
        }

        void InputManager::OnMouseMoveEvent(float x, float y) {
            GetMouse().OnMouseMoveEvent(x, y);
        }

        void InputManager::OnMouseScrollEvent(float deltaX, float deltaY) {
            GetMouse().OnMouseScrollEvent(deltaX, deltaY);
        }
        
        void InputManager::Update() {
            GetKeyboard().Update();
            GetMouse().Update();
        }
        
        void InputManager::ClearBindings() {
            GetKeyboard().ClearBindings();
            GetMouse().ClearBindings();
        }
    }
}
