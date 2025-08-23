#pragma once

#include "../UI/Effect.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class TypewriterEffect : public TextEffect {
            public:
                TypewriterEffect(float charsPerSecond = 10.0f, bool showCursor = true, float startDelay = 0.0f)
                    : TextEffect(-1.0f, 1.0f, false, startDelay), charsPerSecond(charsPerSecond),
                      showCursor(showCursor), cursorBlinkRate(2.0f) {}

                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;

                // Typewriter-specific parameters
                void SetCharsPerSecond(float cps) { charsPerSecond = cps; }
                void SetShowCursor(bool show) { showCursor = show; }
                void SetCursorBlinkRate(float rate) { cursorBlinkRate = rate; }

                float GetCharsPerSecond() const { return charsPerSecond; }
                bool GetShowCursor() const { return showCursor; }

            private:
                float charsPerSecond;
                bool showCursor;
                float cursorBlinkRate;
            };
        }
    }
}
