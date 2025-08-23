#pragma once

#include "../UI/Effect.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class PulseEffect : public TextEffect {
            public:
                PulseEffect(float duration = 0.1f, float minScale = 0.9f, float maxScale = 1.1f,
                          float frequency = 0.5f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 0.5f, loop, startDelay), minScale(minScale), maxScale(maxScale),
                      frequency(frequency) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Pulse-specific parameters
                void SetScaleRange(float minS, float maxS) { minScale = minS; maxScale = maxS; }
                void SetFrequency(float f) { frequency = f; }

                // Get current scale for external use (e.g., Box rendering)
                float GetCurrentScale() const {
                    if (!HasStarted()) return 1.0f;
                    float effectTime = GetEffectTime();
                    float phase = effectTime * frequency * 2.0f * M_PI;
                    float oscillation = (std::sin(phase) + 1.0f) * 0.5f; // 0-1 range
                    return minScale + oscillation * (maxScale - minScale);
                }

            private:
                float minScale;     // Minimum scale multiplier
                float maxScale;     // Maximum scale multiplier
                float frequency;    // Pulse frequency in Hz
            };
        }
    }
}
