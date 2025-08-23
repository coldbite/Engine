#include "MouseInput.h"
#include <algorithm>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace Engine {
    namespace Input {

        MouseInput& MouseInput::GetInstance() {
            static MouseInput instance;
            return instance;
        }

        void MouseInput::BindMouseButton(MouseButton button, MouseButtonCallback callback) {
            BindMouseButton(button, MouseAction::PRESS, callback);
        }

        void MouseInput::BindMouseButton(MouseButton button, MouseAction action, MouseButtonCallback callback) {
            MouseButtonBinding binding;
            binding.action = action;
            binding.callback = callback;
            buttonBindings[button].push_back(binding);
        }

        void MouseInput::UnbindMouseButton(MouseButton button) {
            buttonBindings.erase(button);
        }

        void MouseInput::UnbindMouseButton(MouseButton button, MouseAction action) {
            auto it = buttonBindings.find(button);
            if (it != buttonBindings.end()) {
                auto& bindings = it->second;
                bindings.erase(
                    std::remove_if(bindings.begin(), bindings.end(),
                        [action](const MouseButtonBinding& binding) {
                            return binding.action == action;
                        }),
                    bindings.end()
                );

                if (bindings.empty()) {
                    buttonBindings.erase(it);
                }
            }
        }

        void MouseInput::BindMouseMove(MouseMoveCallback callback) {
            moveCallback = callback;
        }

        void MouseInput::UnbindMouseMove() {
            moveCallback = nullptr;
        }

        void MouseInput::BindMouseScroll(MouseScrollCallback callback) {
            scrollCallback = callback;
        }

        void MouseInput::UnbindMouseScroll() {
            scrollCallback = nullptr;
        }

        bool MouseInput::IsButtonPressed(MouseButton button) const {
            auto currentIt = currentButtonStates.find(button);
            auto previousIt = previousButtonStates.find(button);

            bool currentlyDown = (currentIt != currentButtonStates.end() && currentIt->second);
            bool previouslyDown = (previousIt != previousButtonStates.end() && previousIt->second);

            return currentlyDown && !previouslyDown;
        }

        bool MouseInput::IsButtonDown(MouseButton button) const {
            auto it = currentButtonStates.find(button);
            return (it != currentButtonStates.end() && it->second);
        }

        bool MouseInput::IsButtonReleased(MouseButton button) const {
            auto currentIt = currentButtonStates.find(button);
            auto previousIt = previousButtonStates.find(button);

            bool currentlyDown = (currentIt != currentButtonStates.end() && currentIt->second);
            bool previouslyDown = (previousIt != previousButtonStates.end() && previousIt->second);

            return !currentlyDown && previouslyDown;
        }

        void MouseInput::OnMouseButtonEvent(int platformButton, int platformAction, float x, float y) {
            MouseButton button = ConvertPlatformButton(platformButton);
            MouseAction action = ConvertPlatformAction(platformAction);

            // Update button state
            if (action == MouseAction::PRESS) {
                currentButtonStates[button] = true;
            } else if (action == MouseAction::RELEASE) {
                currentButtonStates[button] = false;
            }

            // Update mouse position
            currentPosition.x = x;
            currentPosition.y = y;

            // Trigger callbacks
            auto it = buttonBindings.find(button);
            if (it != buttonBindings.end()) {
                for (const auto& binding : it->second) {
                    if (binding.action == action && binding.callback) {
                        binding.callback(button, action, x, y);
                    }
                }
            }
        }

        void MouseInput::OnMouseMoveEvent(float x, float y) {
            // Calculate delta
            mouseDelta.deltaX = x - currentPosition.x;
            mouseDelta.deltaY = y - currentPosition.y;

            // Update position
            currentPosition.x = x;
            currentPosition.y = y;

            // Trigger move callback
            if (moveCallback) {
                moveCallback(x, y, mouseDelta.deltaX, mouseDelta.deltaY);
            }
        }

        void MouseInput::OnMouseScrollEvent(float deltaX, float deltaY) {
            // Trigger scroll callback
            if (scrollCallback) {
                scrollCallback(deltaX, deltaY);
            }
        }

        void MouseInput::Update() {
            // Save previous frame's button states
            previousButtonStates = currentButtonStates;

            // Save previous position
            previousPosition = currentPosition;

            // Reset delta (will be updated by next move event)
            mouseDelta.deltaX = 0.0f;
            mouseDelta.deltaY = 0.0f;
        }

        void MouseInput::ClearBindings() {
            buttonBindings.clear();
            moveCallback = nullptr;
            scrollCallback = nullptr;
        }

        MouseButton MouseInput::ConvertPlatformButton(int platformButton) const {
#ifdef _WIN32
            switch (platformButton) {
                case 0: return MouseButton::LEFT;   // WM_LBUTTONDOWN/UP
                case 1: return MouseButton::RIGHT;  // WM_RBUTTONDOWN/UP
                case 2: return MouseButton::MIDDLE; // WM_MBUTTONDOWN/UP
                default: return MouseButton::LEFT;
            }
#else
            // For other platforms
            switch (platformButton) {
                case 0: return MouseButton::LEFT;
                case 1: return MouseButton::MIDDLE; // Different order on some platforms
                case 2: return MouseButton::RIGHT;
                default: return MouseButton::LEFT;
            }
#endif
        }

        MouseAction MouseInput::ConvertPlatformAction(int platformAction) const {
#ifdef _WIN32
            switch (platformAction) {
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                case WM_MBUTTONDOWN:
                    return MouseAction::PRESS;
                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                case WM_MBUTTONUP:
                    return MouseAction::RELEASE;
                default:
                    return MouseAction::PRESS;
            }
#else
            // For other platforms
            if (platformAction == 1) return MouseAction::PRESS;
            return MouseAction::RELEASE;
#endif
        }
    }
}