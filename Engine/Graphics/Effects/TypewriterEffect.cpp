#include "TypewriterEffect.h"
#include "../Text.h"
#include "../IRenderingAPI.h"
#include "../TextAnimator.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void TypewriterEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                // Legacy method - kept for compatibility
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }
            
            void TypewriterEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                // Calculate how many characters should be visible
                int visibleChars = static_cast<int>(time * charsPerSecond);
                
                // This character should be visible if its index is less than visibleChars
                bool isVisible = charIndex < visibleChars;
                
                // Cursor logic - show cursor after last visible character
                bool shouldShowCursor = showCursor && 
                                       charIndex == visibleChars && 
                                       (static_cast<int>(time * cursorBlinkRate) % 2 == 0);
                
                // Control character visibility
                if (!isVisible) {
                    renderState.visible = false;
                }
                
                // TODO: Add cursor rendering logic - would need to add cursor character after this one
                (void)shouldShowCursor; // For now, just suppress warning
            }
        }
    }
}