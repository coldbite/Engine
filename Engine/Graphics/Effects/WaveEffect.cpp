#include "WaveEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void WaveEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void WaveEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Wave calculation: sin(time * frequency + charIndex * phase) * amplitude
                float phase = 0.3f; // Phase offset between characters
                float waveOffset = std::sin((time * frequency) + (charIndex * phase)) * intensity;

                // Apply vertical offset to create wave motion
                renderState.y += waveOffset;
            }
        }
    }
}
