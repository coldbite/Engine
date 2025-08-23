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
                    colors.push_back(new RGBA(255, 0, 0, 100)); // Red
                    colors.push_back(new RGBA(255, 127, 0, 100)); // Orange
                    colors.push_back(new RGBA(255, 255, 0, 100)); // Yellow
                    colors.push_back(new RGBA(0, 255, 0, 100)); // Green
                    colors.push_back(new RGBA(0, 0, 255, 100)); // Blue
                    colors.push_back(new RGBA(127, 0, 255, 100)); // Purple
                }

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Color cycle-specific parameters
                void SetFrequency(float f) { frequency = f; }
                void SetCharDelay(float delay) { charDelay = delay; }
                void SetColors(const std::vector<IColor*>& newColors) { colors = newColors; }

            private:
                float frequency;                // Color cycle frequency
                float charDelay;                // Delay between character color changes
                std::vector<IColor*> colors;       // Colors to cycle through
            };
        }
    }
}
