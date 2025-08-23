#include "BlinkEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void BlinkEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void BlinkEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)charIndex;
                (void)time;
                
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                
                // Calculate blink alpha (0 = invisible, 1 = visible)
                float phase = effectTime * frequency * 2.0f * M_PI;
                float alpha = (std::sin(phase) + 1.0f) > 1.0f ? 1.0f : 0.0f; // Sharp on/off blink
                
                // Apply alpha to character color
                renderState.color = new RGBA(renderState.color->GetRed() * 255,
                                           renderState.color->GetGreen() * 255,
                                           renderState.color->GetBlue() * 255,
                                           renderState.color->GetAlpha() * alpha * 100);
            }
        }
    }
}