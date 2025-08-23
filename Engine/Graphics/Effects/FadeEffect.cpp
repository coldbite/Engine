#include "FadeEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"
#include <algorithm>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void FadeInEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void FadeInEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate fade alpha (0.0 to 1.0)
                float alpha = (duration > 0.0f) ? std::min(time / duration, 1.0f) : 1.0f;

                // Apply alpha to character color
                renderState.color = RGBA(renderState.color.GetRed(),
                                       renderState.color.GetGreen(),
                                       renderState.color.GetBlue(),
                                       renderState.color.GetAlpha() * alpha);
            }

            void FadeOutEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void FadeOutEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate fade alpha (1.0 to 0.0)
                float alpha = (duration > 0.0f) ? std::max(1.0f - (time / duration), 0.0f) : 0.0f;

                // Apply alpha to character color
                renderState.color = RGBA(renderState.color.GetRed(),
                                       renderState.color.GetGreen(),
                                       renderState.color.GetBlue(),
                                       renderState.color.GetAlpha() * alpha);
            }
        }
    }
}
