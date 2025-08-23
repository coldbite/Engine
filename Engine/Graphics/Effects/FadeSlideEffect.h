#pragma once

#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class FadeSlideEffect : public TextEffect {
            public:
                FadeSlideEffect(float duration = 3.0f, float slideDistance = 150.0f,
                              float charDelay = 0.0f, bool fadeOut = true, bool loop = true, float startDelay = 0.0f)
                    : TextEffect(duration, 1.0f, loop, startDelay), slideDistance(slideDistance),
                      charDelay(charDelay), fadeOut(fadeOut) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // FadeSlide-specific parameters
                void SetSlideDistance(float dist) { slideDistance = dist; }
                void SetCharDelay(float delay) { charDelay = delay; }
                void SetFadeOut(bool out) { fadeOut = out; }

            private:
                float slideDistance;    // Distance to slide to the right
                float charDelay;        // Delay between character animations
                bool fadeOut;           // true = fade out, false = fade in

                float EaseInOutQuad(float t) {
                    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
                }
            };
        }
    }
}
