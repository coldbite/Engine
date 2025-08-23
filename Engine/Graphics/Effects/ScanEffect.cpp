#include "ScanEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void ScanEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void ScanEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)charIndex;
                (void)time;
                
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                
                // Scan movement: continuously move from left to right and back
                // Using sawtooth wave: goes from -scanWidth/2 to +scanWidth/2 linearly, then jumps back
                float period = 1.0f / frequency;
                float t = fmod(effectTime, period) / period; // 0 to 1
                
                // Create sawtooth: 0->1->0 over one period
                float sawtoothT;
                if (t < 0.5f) {
                    sawtoothT = t * 2.0f; // 0 to 1 in first half
                } else {
                    sawtoothT = 2.0f - (t * 2.0f); // 1 to 0 in second half
                }
                
                // Map to position: -scanWidth/2 to +scanWidth/2
                float offset = (sawtoothT - 0.5f) * scanWidth;
                
                // Apply horizontal offset
                renderState.x += offset;
            }
        }
    }
}