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
                (void) charIndex;

                if (colors.empty()) {
                    return;
                }

                // Smooth color transition across all characters (no character delay for ambience)
                float colorIndex = (time * frequency) * colors.size();
                int baseIndex = static_cast<int>(colorIndex) % colors.size();
                int nextIndex = (baseIndex + 1) % colors.size();

                // Interpolate between colors for smooth transition
                float t = colorIndex - std::floor(colorIndex);
                IColor* color1 = colors[baseIndex];
                IColor* color2 = colors[nextIndex];

                // Apply subtle ambience color multiplication
                float ambienceR = color1->GetRed() + (color2->GetRed() - color1->GetRed()) * t;
                float ambienceG = color1->GetGreen() + (color2->GetGreen() - color1->GetGreen()) * t;
                float ambienceB = color1->GetBlue() + (color2->GetBlue() - color1->GetBlue()) * t;

                // Multiply with existing color for ambience effect
                renderState.color = new RGBA(
                    renderState.color->GetRed() * ambienceR * 255,
                    renderState.color->GetGreen() * ambienceG * 255,
                    renderState.color->GetBlue() * ambienceB * 255,
                    renderState.color->GetAlpha() * 100
                );
            }
        }
    }
}
