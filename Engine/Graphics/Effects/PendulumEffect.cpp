#include "PendulumEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void PendulumEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void PendulumEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)charIndex;
                (void)time;
                
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                
                // Pendulum movement: Start at center (0), move right, then left, then back to center
                // Using sine so it starts at 0 (center), goes to +1 (right), then -1 (left), then back to 0
                float phase = effectTime * frequency * 2.0f * M_PI;
                float offset = distance * std::sin(phase);
                
                // Apply horizontal offset (positive = right, negative = left)
                renderState.x += offset;
            }
        }
    }
}