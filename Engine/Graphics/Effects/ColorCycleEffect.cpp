#include "ColorCycleEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void ColorCycleEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void ColorCycleEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                if (colors.empty()) {
                    return;
                }

                // Calculate color cycle with character delay
                float charTime = time - (charIndex * charDelay);

                // Don't animate characters that haven't started yet
                if (charTime < 0.0f) {
                    return;
                }

                // Calculate which color to use
                float colorIndex = (charTime * frequency) * colors.size();
                int baseIndex = static_cast<int>(colorIndex) % colors.size();
                int nextIndex = (baseIndex + 1) % colors.size();

                // Interpolate between colors for smooth transition
                float t = colorIndex - std::floor(colorIndex);
                IColor* color1 = colors[baseIndex];
                IColor* color2 = colors[nextIndex];

                renderState.color = new RGBA(
                    (color1->GetRed() + (color2->GetRed() - color1->GetRed()) * t) * 255,
                    (color1->GetGreen() + (color2->GetGreen() - color1->GetGreen()) * t) * 255,
                    (color1->GetBlue() + (color2->GetBlue() - color1->GetBlue()) * t) * 255,
                    renderState.color->GetAlpha() * 100 // Preserve original alpha
                );
            }
        }
    }
}
