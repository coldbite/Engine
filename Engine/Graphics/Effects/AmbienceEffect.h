#pragma once

#include "../UI/Effect.h"
#include "../RGBA.h"
#include <cmath>
#include <vector>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class AmbienceEffect : public TextEffect {
            public:
                AmbienceEffect(float duration = -1.0f, float frequency = 0.5f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), frequency(frequency) {
                    // Subtle ambience colors - warm to cool tones
                    colors.push_back(new RGBA(255, 229, 204, 100));   // Warm white
                    colors.push_back(new RGBA(229, 242, 255, 100));  // Cool white
                    colors.push_back(new RGBA(255, 242, 229, 100));  // Soft warm
                    colors.push_back(new RGBA(242, 229, 255, 100));  // Soft cool
                }

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Ambience-specific parameters
                void SetFrequency(float f) { frequency = f; }
                void SetAmbienceColors(const std::vector<IColor*>& newColors) { colors = newColors; }

            private:
                float frequency;                // Color transition frequency
                std::vector<IColor*> colors;       // Ambience colors to cycle through
            };
        }
    }
}
