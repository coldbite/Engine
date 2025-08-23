#include "TravelEffect.h"
#include "../UI/Text/Text.h"
#include "../IRenderingAPI.h"
#include "../UI/Animator.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            void TravelEffect::Apply(Text& text, IRenderingAPI& context, float time, int charIndex) {
                (void)text;
                (void)context;
                (void)time;
                (void)charIndex;
            }

            void TravelEffect::ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) {
                (void)charIndex;
                (void)time;
                
                if (!HasStarted()) return;
                
                float effectTime = GetEffectTime();
                float progress = (duration > 0.0f) ? std::min(effectTime / duration, 1.0f) : 1.0f;
                
                float offset;
                if (progress <= 0.5f) {
                    // Erste Hälfte: Von 0 nach -travelDistance (nach links)
                    float halfProgress = progress * 2.0f; // 0 to 1
                    offset = -travelDistance * EaseInOutQuad(halfProgress);
                } else {
                    // Zweite Hälfte: Von -travelDistance zurück nach 0 (nach rechts)
                    float halfProgress = (progress - 0.5f) * 2.0f; // 0 to 1
                    offset = -travelDistance * (1.0f - EaseInOutQuad(halfProgress));
                }
                
                renderState.x += offset;
            }
        }
    }
}