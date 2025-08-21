#pragma once

#include "../TextEffect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            enum class SlideDirection {
                LEFT,
                RIGHT,
                TOP,
                BOTTOM
            };

            class SlideEffect : public TextEffect {
            public:
                SlideEffect(float duration = 2.0f, SlideDirection dir = SlideDirection::LEFT,
                          float distance = 100.0f, float delay = 0.1f)
                    : TextEffect(duration, 1.0f, false), direction(dir),
                      slideDistance(distance), charDelay(0.0f) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Slide-specific parameters
                void SetDirection(SlideDirection dir) { direction = dir; }
                void SetDistance(float dist) { slideDistance = dist; }
                void SetCharDelay(float delay) { charDelay = delay; }

            private:
                SlideDirection direction;   // Direction to slide from
                float slideDistance;        // Distance to slide from
                float charDelay;           // Delay between character slides

                float EaseOutCubic(float t) {
                    return 1.0f - std::pow(1.0f - t, 3.0f);
                }
            };
        }
    }
}
