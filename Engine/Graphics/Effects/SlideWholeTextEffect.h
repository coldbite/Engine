#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            enum class SlideDirection;

            class SlideWholeTextEffect : public TextEffect {
            public:
                SlideWholeTextEffect(float duration = 2.0f, SlideDirection dir = SlideDirection::LEFT,
                                   float distance = 100.0f, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, false, startDelay), direction(dir),
                      slideDistance(distance) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // SlideWholeText-specific parameters
                void SetDirection(SlideDirection dir) { direction = dir; }
                void SetDistance(float dist) { slideDistance = dist; }

            private:
                SlideDirection direction;   // Direction to slide from
                float slideDistance;        // Distance to slide from

                float EaseOutCubic(float t) {
                    return 1.0f - std::pow(1.0f - t, 3.0f);
                }
            };
        }
    }
}