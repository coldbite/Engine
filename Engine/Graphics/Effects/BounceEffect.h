#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class BounceEffect : public TextEffect {
            public:
                BounceEffect(float duration = -1.0f, float intensity = 20.0f,
                           float frequency = 3.0f, bool loop = true, float charDelay = 0.1f, float startDelay = 0.0f)
                    : TextEffect(duration, intensity, loop, startDelay), frequency(frequency), charDelay(charDelay) {}

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
