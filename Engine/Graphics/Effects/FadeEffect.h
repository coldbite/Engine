#pragma once

#include "../TextEffect.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            class FadeInEffect : public TextEffect {
            public:
                FadeInEffect(float duration = 1.0f)
                    : TextEffect(duration, 1.0f, false) {}
                
                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;
            };
            
            class FadeOutEffect : public TextEffect {
            public:
                FadeOutEffect(float duration = 1.0f)
                    : TextEffect(duration, 1.0f, false) {}
                
                void Apply(Text& text, IRenderingAPI& context, float time, int charIndex) override;
                void ApplyToCharacter(CharacterRenderState& renderState, int charIndex, float time) override;
            };
        }
    }
}