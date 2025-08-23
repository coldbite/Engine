#include "AmbienceEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void AmbienceEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void AmbienceEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                if (colors.empty()) {
                    return;
                }

                // Smooth color transition across all characters (no character delay for ambience)
                float colorIndex = (time * frequency) * colors.size();
                int baseIndex = static_cast<int>(colorIndex) % colors.size();
                int nextIndex = (baseIndex + 1) % colors.size();

                // Interpolate between colors for smooth transition
                float t = colorIndex - std::floor(colorIndex);
                const RGBA& color1 = colors[baseIndex];
                const RGBA& color2 = colors[nextIndex];

                // Apply subtle ambience color multiplication
                float ambienceR = color1.GetRed() + (color2.GetRed() - color1.GetRed()) * t;
                float ambienceG = color1.GetGreen() + (color2.GetGreen() - color1.GetGreen()) * t;
                float ambienceB = color1.GetBlue() + (color2.GetBlue() - color1.GetBlue()) * t;

                // Multiply with existing color for ambience effect
                renderState.color = RGBA(
                    renderState.color.GetRed() * ambienceR,
                    renderState.color.GetGreen() * ambienceG,
                    renderState.color.GetBlue() * ambienceB,
                    renderState.color.GetAlpha()
                );
            }
        }
    }
}
