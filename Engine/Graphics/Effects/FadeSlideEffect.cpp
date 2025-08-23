#include "FadeSlideEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void FadeSlideEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void FadeSlideEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate progress for the entire text block (no character delay)
                float progress = (duration > 0.0f) ? std::min(time / duration, 1.0f) : 1.0f;

                // Apply easing for smooth animation
                progress = EaseInOutQuad(progress);

                // Apply fade effect to all characters equally
                float alpha;
                if (fadeOut) {
                    alpha = 1.0f - progress; // Start visible, end invisible
                } else {
                    alpha = progress; // Start invisible, end visible
                }

                renderState.color = new RGBA(renderState.color->GetRed(),
                                       renderState.color->GetGreen(),
                                       renderState.color->GetBlue(),
                                       renderState.color->GetAlpha() * alpha);

                // Apply slide to the right for entire text block
                // All characters move together as one unit
                float slideOffset = slideDistance * progress;
                renderState.x += slideOffset;
            }
        }
    }
}
