#pragma once

#include "../UI/Effect.h"
#include "../RGBA.h"
#include <cmath>
#include <vector>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class ColorCycleEffect : public TextEffect {
            public:
                ColorCycleEffect(float duration = -1.0f, float frequency = 1.0f,
                               bool loop = true, float charDelay = 0.05f, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), frequency(frequency), charDelay(charDelay) {
                    // Default rainbow colors
                    colors.push_back(RGBA(1.0f, 0.0f, 0.0f, 1.0f)); // Red
                    colors.push_back(RGBA(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
                    colors.push_back(RGBA(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow
                    colors.push_back(RGBA(0.0f, 1.0f, 0.0f, 1.0f)); // Green
                    colors.push_back(RGBA(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
                    colors.push_back(RGBA(0.5f, 0.0f, 1.0f, 1.0f)); // Purple
                }

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Color cycle-specific parameters
                void SetFrequency(float f) { frequency = f; }
                void SetCharDelay(float delay) { charDelay = delay; }
                void SetColors(const std::vector<RGBA>& newColors) { colors = newColors; }

            private:
                float frequency;                // Color cycle frequency
                float charDelay;                // Delay between character color changes
                std::vector<RGBA> colors;       // Colors to cycle through
            };
        }
    }
}
