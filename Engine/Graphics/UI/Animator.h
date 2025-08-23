#pragma once

#include "Effect.h"
#include "../RGBA.h"
#include <memory>
#include <vector>

namespace Engine {
    namespace Graphics {
        class Text;
        class IRenderingAPI;

        // Struct to hold character rendering state for effects
        struct CharacterRenderState {
            float x, y;              // Position
            float width, height;     // Dimensions
            RGBA color;              // Color
            bool visible;            // Visibility flag
            float scale;             // Scale multiplier
            float rotation;          // Rotation in degrees

            CharacterRenderState()
                : x(0), y(0), width(0), height(0),
                  color(1.0f, 1.0f, 1.0f, 1.0f),
                  visible(true), scale(1.0f), rotation(0.0f) {}
        };

        class TextAnimator {
        public:
            TextAnimator() = default;
            ~TextAnimator() = default;

            // Attach to a text object
            void AttachTo(Text* text) { targetText = text; }

            // Add effects
            void AddEffect(std::unique_ptr<TextEffect> effect);

            // Update all effects
            void Update(float deltaTime);

            // Apply all effects during text rendering
            void ApplyEffects(IRenderingAPI& context, const std::string& text,
                             float baseX, float baseY);

            // Apply effects to a single character during rendering
            void ApplyEffectsToCharacter(IRenderingAPI& context, char character,
                                       int charIndex, CharacterRenderState& renderState);

            // Clear all effects
            void ClearEffects() { effects.clear(); }

            // Remove finished effects
            void RemoveFinishedEffects();

            // Check if any effects are active
            bool HasActiveEffects() const;

            // Get number of effects
            size_t GetEffectCount() const { return effects.size(); }

        private:
            Text* targetText = nullptr;
            std::vector<std::unique_ptr<TextEffect>> effects;

            // Helper to calculate character positions
            std::vector<CharacterInfo> CalculateCharacterPositions(const std::string& text,
                                                                  float baseX, float baseY);
        };
    }
}
