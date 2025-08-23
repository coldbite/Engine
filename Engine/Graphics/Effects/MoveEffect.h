#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            enum class MoveDirection {
                LEFT,
                RIGHT,
                UP,
                DOWN
            };

            class MoveEffect : public TextEffect {
            public:
                MoveEffect(float duration = 2.0f, MoveDirection dir = MoveDirection::LEFT,
                         float distance = 100.0f, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, false, startDelay), direction(dir),
                      moveDistance(distance) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                void SetDirection(MoveDirection dir) { direction = dir; }
                void SetDistance(float dist) { moveDistance = dist; }

            private:
                MoveDirection direction;
                float moveDistance;
                
                float EaseInOutQuad(float t) {
                    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
                }
            };
        }
    }
}