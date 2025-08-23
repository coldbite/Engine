#pragma once

#include "../UI/Effect.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class PendulumEffect : public TextEffect {
            public:
                PendulumEffect(float duration = -1.0f, float distance = 100.0f, 
                             float frequency = 0.5f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), 
                      distance(distance), frequency(frequency) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Pendulum-specific parameters
                void SetDistance(float dist) { distance = dist; }
                void SetFrequency(float freq) { frequency = freq; }

            private:
                float distance;     // Distance to move left/right
                float frequency;    // How fast to swing (Hz)
            };
        }
    }
}