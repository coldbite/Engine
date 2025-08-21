#pragma once

#include "../TextEffect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class BounceEffect : public TextEffect {
            public:
                BounceEffect(float duration = -1.0f, float intensity = 20.0f, 
                           float frequency = 3.0f, bool loop = true, float delay = 0.1f)
                    : TextEffect(duration, intensity, loop), frequency(frequency), charDelay(delay) {}
                
                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;
                
                // Bounce-specific parameters
                void SetFrequency(float f) { frequency = f; }
                void SetCharDelay(float delay) { charDelay = delay; }
                float GetFrequency() const { return frequency; }
                float GetCharDelay() const { return charDelay; }
                
            private:
                float frequency;    // Bounce frequency
                float charDelay;    // Delay between character bounces
            };
        }
    }
}