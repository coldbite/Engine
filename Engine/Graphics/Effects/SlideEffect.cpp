#include "SlideEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void SlideEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void SlideEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate slide progress with character delay
                float charTime = time - (charIndex * charDelay);

                // Don't animate characters that haven't started yet
                if (charTime < 0.0f) {
                    // Character hasn't started animating - keep it at start position
                    switch (direction) {
                        case SlideDirection::LEFT:
                            renderState.x -= slideDistance;
                            break;
                        case SlideDirection::RIGHT:
                            renderState.x += slideDistance;
                            break;
                        case SlideDirection::TOP:
                            renderState.y -= slideDistance;
                            break;
                        case SlideDirection::BOTTOM:
                            renderState.y += slideDistance;
                            break;
                    }
                    return;
                }

                // Calculate slide progress (0 = start position, 1 = final position)
                float progress = (duration > 0.0f) ? std::min(charTime / duration, 1.0f) : 1.0f;

                // Apply easing for smooth animation
                progress = EaseOutCubic(progress);

                // Calculate offset based on remaining distance to slide
                float offset = slideDistance * (1.0f - progress);

                // Apply offset based on direction
                switch (direction) {
                    case SlideDirection::LEFT:
                        renderState.x -= offset;
                        break;
                    case SlideDirection::RIGHT:
                        renderState.x += offset;
                        break;
                    case SlideDirection::TOP:
                        renderState.y -= offset;
                        break;
                    case SlideDirection::BOTTOM:
                        renderState.y += offset;
                        break;
                }
            }
        }
    }
}
