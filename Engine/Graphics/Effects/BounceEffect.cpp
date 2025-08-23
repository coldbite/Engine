#include "BounceEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void BounceEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void BounceEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate bounce with character delay
                float charTime = time - (charIndex * charDelay);

                // Don't animate characters that haven't started yet
                if (charTime < 0.0f) {
                    return;
                }

                // Create bounce using sine wave with easing
                float bounce = std::sin(charTime * frequency) * intensity;

                // Apply easing to make bounce look more natural (stronger at peaks)
                float easedBounce = bounce * std::abs(std::sin(charTime * frequency));

                // Only apply downward bounces (negative Y values for downward movement)
                if (easedBounce > 0) {
                    renderState.y -= easedBounce;
                }
            }
        }
    }
}
