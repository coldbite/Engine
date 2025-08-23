#pragma once

#include <string>
#include <vector>

namespace Engine {
    namespace Graphics {
        class Text;
        class IRenderingAPI;
        struct CharacterRenderState;
        
        // Base class for all text effects
        class TextEffect {
        public:
            TextEffect(float duration = 1.0f, float intensity = 1.0f, bool loop = false, float startDelay = 0.0f)
                : duration(duration), intensity(intensity), loop(loop), startDelay(startDelay), currentTime(0.0f) {}
            
            virtual ~TextEffect() = default;
            
            // Update the effect with delta time
            virtual void Update(float deltaTime);
            
            // Apply the effect to text rendering
            virtual void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) = 0;
            
            // Apply the effect to a single character's render state
            virtual void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)renderState; (void)charIndex; (void)time;
            }
            
            // Check if effect is finished
            bool IsFinished() const { return !loop && currentTime >= (duration + startDelay); }
            
            // Check if effect has started (after delay)
            bool HasStarted() const { return currentTime >= startDelay; }
            
            // Get the effective time for the effect (accounting for delay)
            float GetEffectTime() const { return HasStarted() ? (currentTime - startDelay) : 0.0f; }
            
            // Effect properties
            float GetDuration() const { return duration; }
            float GetIntensity() const { return intensity; }
            float GetCurrentTime() const { return currentTime; }
            float GetStartDelay() const { return startDelay; }
            bool IsLooping() const { return loop; }
            
            // Setters
            void SetDuration(float d) { duration = d; }
            void SetIntensity(float i) { intensity = i; }
            void SetLoop(bool l) { loop = l; }
            void SetStartDelay(float delay) { startDelay = delay; }
            void Reset() { currentTime = 0.0f; }
            
        protected:
            float duration;     // Effect duration in seconds (-1 for infinite)
            float intensity;    // Effect intensity multiplier
            bool loop;          // Whether effect should loop
            float startDelay;   // Delay before effect starts in seconds
            float currentTime;  // Current time in effect
        };
        
        // Helper struct for character positioning
        struct CharacterInfo {
            char character;
            float x, y;         // Base position
            float width, height; // Character dimensions
            int index;          // Character index in string
        };
    }
}