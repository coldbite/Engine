#include "InputManager.h"
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace Engine {
    namespace Input {
        
        InputManager& InputManager::GetInstance() {
            static InputManager instance;
            return instance;
        }
        
        void InputManager::BindKey(KEY key, KeyCallback callback) {
            BindKey(key, KeyAction::PRESS, callback);
        }
        
        void InputManager::BindKey(KEY key, KeyAction action, KeyCallback callback) {
            KeyBinding binding;
            binding.action = action;
            binding.callback = callback;
            keyBindings[key].push_back(binding);
        }
        
        void InputManager::UnbindKey(KEY key) {
            keyBindings.erase(key);
        }
        
        void InputManager::UnbindKey(KEY key, KeyAction action) {
            auto it = keyBindings.find(key);
            if (it != keyBindings.end()) {
                auto& bindings = it->second;
                size_t sizeBefore = bindings.size();
                
                bindings.erase(
                    std::remove_if(bindings.begin(), bindings.end(),
                        [action](const KeyBinding& binding) {
                            return binding.action == action;
                        }),
                    bindings.end()
                );
                
                
                if (bindings.empty()) {
                    keyBindings.erase(it);
                }
            }
        }
        
        bool InputManager::IsKeyPressed(KEY key) const {
            auto currentIt = currentKeyStates.find(key);
            auto previousIt = previousKeyStates.find(key);
            
            bool currentlyDown = (currentIt != currentKeyStates.end() && currentIt->second);
            bool previouslyDown = (previousIt != previousKeyStates.end() && previousIt->second);
            
            return currentlyDown && !previouslyDown;
        }
        
        bool InputManager::IsKeyDown(KEY key) const {
            auto it = currentKeyStates.find(key);
            return (it != currentKeyStates.end() && it->second);
        }
        
        bool InputManager::IsKeyReleased(KEY key) const {
            auto currentIt = currentKeyStates.find(key);
            auto previousIt = previousKeyStates.find(key);
            
            bool currentlyDown = (currentIt != currentKeyStates.end() && currentIt->second);
            bool previouslyDown = (previousIt != previousKeyStates.end() && previousIt->second);
            
            return !currentlyDown && previouslyDown;
        }
        
        void InputManager::OnKeyEvent(int platformKey, int platformAction) {
            KEY key = ConvertPlatformKey(platformKey);
            KeyAction action = ConvertPlatformAction(platformAction);
            
            // Update key state
            if (action == KeyAction::PRESS || action == KeyAction::REPEAT) {
                currentKeyStates[key] = true;
            } else if (action == KeyAction::RELEASE) {
                currentKeyStates[key] = false;
            }
            
            // Trigger callbacks
            auto it = keyBindings.find(key);
            if (it != keyBindings.end()) {
                for (const auto& binding : it->second) {
                    if (binding.action == action || 
                        (binding.action == KeyAction::PRESS && action == KeyAction::REPEAT)) {
                        if (binding.callback) {
                            binding.callback(key, action);
                        }
                    }
                }
            }
        }
        
        void InputManager::Update() {
            // Save previous frame's key states
            previousKeyStates = currentKeyStates;
        }
        
        void InputManager::ClearBindings() {
            keyBindings.clear();
        }
        
        KEY InputManager::ConvertPlatformKey(int platformKey) const {
#ifdef _WIN32
            // Windows virtual key codes to our KEY enum
            switch (platformKey) {
                case VK_ESCAPE: return KEY::ESC;
                case VK_RETURN: return KEY::ENTER;
                case VK_SPACE: return KEY::SPACE;
                case VK_TAB: return KEY::TAB;
                case VK_BACK: return KEY::BACKSPACE;
                case VK_DELETE: return KEY::DELETE_KEY;
                
                case VK_UP: return KEY::UP;
                case VK_DOWN: return KEY::DOWN;
                case VK_LEFT: return KEY::LEFT;
                case VK_RIGHT: return KEY::RIGHT;
                
                case VK_F1: return KEY::F1;
                case VK_F2: return KEY::F2;
                case VK_F3: return KEY::F3;
                case VK_F4: return KEY::F4;
                case VK_F5: return KEY::F5;
                case VK_F6: return KEY::F6;
                case VK_F7: return KEY::F7;
                case VK_F8: return KEY::F8;
                case VK_F9: return KEY::F9;
                case VK_F10: return KEY::F10;
                case VK_F11: return KEY::F11;
                case VK_F12: return KEY::F12;
                
                case '0': return KEY::NUM_0;
                case '1': return KEY::NUM_1;
                case '2': return KEY::NUM_2;
                case '3': return KEY::NUM_3;
                case '4': return KEY::NUM_4;
                case '5': return KEY::NUM_5;
                case '6': return KEY::NUM_6;
                case '7': return KEY::NUM_7;
                case '8': return KEY::NUM_8;
                case '9': return KEY::NUM_9;
                
                case 'A': return KEY::A;
                case 'B': return KEY::B;
                case 'C': return KEY::C;
                case 'D': return KEY::D;
                case 'E': return KEY::E;
                case 'F': return KEY::F;
                case 'G': return KEY::G;
                case 'H': return KEY::H;
                case 'I': return KEY::I;
                case 'J': return KEY::J;
                case 'K': return KEY::K;
                case 'L': return KEY::L;
                case 'M': return KEY::M;
                case 'N': return KEY::N;
                case 'O': return KEY::O;
                case 'P': return KEY::P;
                case 'Q': return KEY::Q;
                case 'R': return KEY::R;
                case 'S': return KEY::S;
                case 'T': return KEY::T;
                case 'U': return KEY::U;
                case 'V': return KEY::V;
                case 'W': return KEY::W;
                case 'X': return KEY::X;
                case 'Y': return KEY::Y;
                case 'Z': return KEY::Z;
                
                case VK_SHIFT: return KEY::SHIFT;
                case VK_CONTROL: return KEY::CTRL;
                case VK_MENU: return KEY::ALT;
                
                default:
                    // Return the platform key as-is if no mapping exists
                    return static_cast<KEY>(platformKey);
            }
#else
            // For other platforms, return as-is for now
            return static_cast<KEY>(platformKey);
#endif
        }
        
        KeyAction InputManager::ConvertPlatformAction(int platformAction) const {
#ifdef _WIN32
            // Windows message types
            switch (platformAction) {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                    return KeyAction::PRESS;
                case WM_KEYUP:
                case WM_SYSKEYUP:
                    return KeyAction::RELEASE;
                default:
                    return KeyAction::PRESS;
            }
#else
            // For other platforms
            if (platformAction == 1) return KeyAction::PRESS;
            if (platformAction == 0) return KeyAction::RELEASE;
            return KeyAction::REPEAT;
#endif
        }
    }
}