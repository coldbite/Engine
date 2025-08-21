#pragma once

#include "../TextEffect.h"
#include "../RGBA.h"
#include <cmath>
#include <vector>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class AmbienceEffect : public TextEffect {
            public:
                AmbienceEffect(float duration = -1.0f, float frequency = 0.5f, bool loop = true)
                    : TextEffect(duration, 1.0f, loop), frequency(frequency) {
                    // Subtle ambience colors - warm to cool tones
                    colors.push_back(RGBA(1.0f, 0.9f, 0.8f, 1.0f));   // Warm white
                    colors.push_back(RGBA(0.9f, 0.95f, 1.0f, 1.0f));  // Cool white
                    colors.push_back(RGBA(1.0f, 0.95f, 0.9f, 1.0f));  // Soft warm
                    colors.push_back(RGBA(0.95f, 0.9f, 1.0f, 1.0f));  // Soft cool
                }
                
                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;
                
                // Ambience-specific parameters
                void SetFrequency(float f) { frequency = f; }
                void SetAmbienceColors(const std::vector<RGBA>& newColors) { colors = newColors; }
                
            private:
                float frequency;                // Color transition frequency
                std::vector<RGBA> colors;       // Ambience colors to cycle through
            };
        }
    }
}