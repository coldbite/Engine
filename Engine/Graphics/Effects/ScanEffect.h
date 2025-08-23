#pragma once

#include "../UI/Effect.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class ScanEffect : public TextEffect {
            public:
                ScanEffect(float duration = -1.0f, float scanWidth = 200.0f, 
                         float frequency = 0.5f, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), 
                      scanWidth(scanWidth), frequency(frequency) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Scan-specific parameters
                void SetScanWidth(float width) { scanWidth = width; }
                void SetFrequency(float freq) { frequency = freq; }

            private:
                float scanWidth;    // Total width to scan across
                float frequency;    // How fast to scan (Hz)
            };
        }
    }
}