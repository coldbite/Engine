#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

#ifdef _WIN32
    #include <windows.h>
    // Fix Windows macro conflicts
    #ifdef RGB
    #undef RGB
    #endif
#endif

namespace Engine {
    namespace Input {
        
        enum class KEY {
            // Special keys
            ESC = 27,
            ENTER = 13,
            SPACE = 32,
            TAB = 9,
            BACKSPACE = 8,
            DELETE_KEY = 46,
            
            // Arrow keys
            UP = 38,
            DOWN = 40,
            LEFT = 37,
            RIGHT = 39,
            
            // Function keys
            F1 = 112, F2 = 113, F3 = 114, F4 = 115,
            F5 = 116, F6 = 117, F7 = 118, F8 = 119,
            F9 = 120, F10 = 121, F11 = 122, F12 = 123,
            
            // Numbers
            NUM_0 = 48, NUM_1 = 49, NUM_2 = 50, NUM_3 = 51, NUM_4 = 52,
            NUM_5 = 53, NUM_6 = 54, NUM_7 = 55, NUM_8 = 56, NUM_9 = 57,
            
            // Letters
            A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
            I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
            Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
            Y = 89, Z = 90,
            
            // Modifiers
            SHIFT = 16,
            CTRL = 17,
            ALT = 18,
            
            // Mouse buttons (for future use)
            MOUSE_LEFT = 1001,
            MOUSE_RIGHT = 1002,
            MOUSE_MIDDLE = 1003
        };

        enum class KeyAction {
            PRESS,
            RELEASE,
            REPEAT
        };

        using KeyCallback = std::function<void(KEY key, KeyAction action)>;

        class InputManager {
        public:
            static InputManager& GetInstance();
            
            // Key binding system
            void BindKey(KEY key, KeyCallback callback);
            void BindKey(KEY key, KeyAction action, KeyCallback callback);
            void UnbindKey(KEY key);
            void UnbindKey(KEY key, KeyAction action);
            
            // Key state queries
            bool IsKeyPressed(KEY key) const;
            bool IsKeyDown(KEY key) const;
            bool IsKeyReleased(KEY key) const;
            
            // Internal event handling (called by NativeWindow)
            void OnKeyEvent(int platformKey, int platformAction);
            void Update(); // Call once per frame to update key states
            
            // Clear all bindings
            void ClearBindings();
            
        private:
            InputManager() = default;
            ~InputManager() = default;
            InputManager(const InputManager&) = delete;
            InputManager& operator=(const InputManager&) = delete;
            
            // Convert platform-specific key codes to our KEY enum
            KEY ConvertPlatformKey(int platformKey) const;
            KeyAction ConvertPlatformAction(int platformAction) const;
            
            // Key binding storage
            struct KeyBinding {
                KeyAction action = KeyAction::PRESS;
                KeyCallback callback;
            };
            
            std::unordered_map<KEY, std::vector<KeyBinding>> keyBindings;
            
            // Key state tracking
            std::unordered_map<KEY, bool> currentKeyStates;
            std::unordered_map<KEY, bool> previousKeyStates;
        };
        
        // Convenience function for global access
        inline InputManager& GetInput() { return InputManager::GetInstance(); }
    }
}