#include "ScaleEffect.h"
#include "../Text.h"
#include "../IRenderingAPI.h"
#include "../TextAnimator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void ScaleEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }
            
            void ScaleEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate scale with character delay
                float charTime = time - (charIndex * charDelay);
                
                // Don't animate characters that haven't started yet
                if (charTime < 0.0f) {
                    return;
                }
                
                // Create scale oscillation using sine wave
                float scaleOffset = (std::sin(charTime * frequency) + 1.0f) * 0.5f; // Normalize to 0-1
                float scale = minScale + (maxScale - minScale) * scaleOffset;
                
                // Apply scale to character
                renderState.scale *= scale;
            }
        }
    }
}