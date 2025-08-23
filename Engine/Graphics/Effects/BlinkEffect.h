#pragma once

#include "../UI/Effect.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class BlinkEffect : public TextEffect {
            public:
                BlinkEffect(float duration = -1.0f, float frequency = 2.0f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), frequency(frequency) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Blink-specific parameters
                void SetFrequency(float f) { frequency = f; }

            private:
                float frequency;    // Blink frequency in Hz
            };
        }
    }
}