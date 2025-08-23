#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class TravelEffect : public TextEffect {
            public:
                TravelEffect(float duration = 3.0f, float distance = 180.0f, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, false, startDelay), travelDistance(distance) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                void SetDistance(float dist) { travelDistance = dist; }

            private:
                float travelDistance;
                
                float EaseInOutQuad(float t) {
                    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
                }
            };
        }
    }
}