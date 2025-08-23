#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class ScaleEffect : public TextEffect {
            public:
                ScaleEffect(float duration = -1.0f, float minScale = 0.8f, float maxScale = 1.2f,
                          float frequency = 2.0f, bool loop = true, float charDelay = 0.1f, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), minScale(minScale), maxScale(maxScale),
                      frequency(frequency), charDelay(charDelay) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Scale-specific parameters
                void SetScaleRange(float minS, float maxS) { minScale = minS; maxScale = maxS; }
                void SetFrequency(float f) { frequency = f; }
                void SetCharDelay(float delay) { charDelay = delay; }

            private:
                float minScale;     // Minimum scale multiplier
                float maxScale;     // Maximum scale multiplier
                float frequency;    // Scale oscillation frequency
                float charDelay;    // Delay between character scaling
            };
        }
    }
}
