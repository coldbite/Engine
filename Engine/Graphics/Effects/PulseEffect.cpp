#include "PulseEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void PulseEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void PulseEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                
                // Calculate pulse scale
                float phase = effectTime * frequency * 2.0f * M_PI;
                float oscillation = (std::sin(phase) + 1.0f) * 0.5f; // 0-1 range
                float scale = minScale + oscillation * (maxScale - minScale);
                
                // Apply scale to character
                renderState.scale *= scale;
            }
        }
    }
}