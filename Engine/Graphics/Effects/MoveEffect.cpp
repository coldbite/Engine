#include "MoveEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void MoveEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void MoveEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)charIndex;
                (void)time;
                
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                float progress = (duration > 0.0f) ? std::min(effectTime / duration, 1.0f) : 1.0f;
                
                // Apply easing
                progress = EaseInOutQuad(progress);
                
                // Calculate offset (moves FROM start position TO target position)
                float offset = moveDistance * progress;
                
                switch (direction) {
                    case MoveDirection::LEFT:
                        renderState.x -= offset;  // Move left (negative X)
                        break;
                    case MoveDirection::RIGHT:
                        renderState.x += offset;  // Move right (positive X)
                        break;
                    case MoveDirection::UP:
                        renderState.y -= offset;  // Move up (negative Y)
                        break;
                    case MoveDirection::DOWN:
                        renderState.y += offset;  // Move down (positive Y)
                        break;
                }
            }
        }
    }
}