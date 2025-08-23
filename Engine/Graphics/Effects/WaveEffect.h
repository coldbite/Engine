#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class WaveEffect : public TextEffect {
            public:
                WaveEffect(float duration = -1.0f, float intensity = 15.0f,
                          float frequency = 2.0f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, intensity, loop, startDelay), frequency(frequency) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Wave-specific parameters
                void SetFrequency(float f) { frequency = f; }
                float GetFrequency() const { return frequency; }

            private:
                float frequency; // Wave frequency multiplier
            };
        }
    }
}
