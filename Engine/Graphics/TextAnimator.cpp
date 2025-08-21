#include "TextAnimator.h"
#include "Text.h"
#include "IRenderingAPI.h"
#include <algorithm>

// Fix Windows macro conflicts
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace Engine {
    namespace Graphics {
        void TextAnimator::AddEffect(std::unique_ptr<TextEffect> effect) {
            if (effect) {
                effects.push_back(std::move(effect));
            }
        }
        
        void TextAnimator::Update(float deltaTime) {
            for (auto& effect : effects) {
                if (effect) {
                    effect->Update(deltaTime);
                }
            }
            
            // Automatically remove finished non-looping effects
            RemoveFinishedEffects();
        }
        
        void TextAnimator::ApplyEffects(IRenderingAPI& context, const std::string& text, 
                                       float baseX, float baseY) {
            if (!targetText || effects.empty()) {
                return;
            }
            
            // Calculate character positions
            auto charPositions = CalculateCharacterPositions(text, baseX, baseY);
            
            // Apply each effect to each character
            for (auto& effect : effects) {
                if (effect && !effect->IsFinished()) {
                    for (size_t i = 0; i < charPositions.size(); ++i) {
                        float currentTime = effect->GetCurrentTime();
                        effect->Apply(*targetText, context, currentTime, 
                                    static_cast<int>(i));
                    }
                }
            }
        }
        
        void TextAnimator::ApplyEffectsToCharacter(IRenderingAPI& context, char character, 
                                                 int charIndex, CharacterRenderState& renderState) {
            if (effects.empty()) {
                return;
            }
            
            // Apply each active effect to this character
            for (auto& effect : effects) {
                if (effect && !effect->IsFinished()) {
                    // Create a specialized Apply method that can modify the render state
                    float currentTime = effect->GetCurrentTime();
                    effect->ApplyToCharacter(renderState, charIndex, currentTime);
                }
            }
        }
        
        void TextAnimator::RemoveFinishedEffects() {
            effects.erase(
                std::remove_if(effects.begin(), effects.end(),
                    [](const std::unique_ptr<TextEffect>& effect) {
                        return effect && effect->IsFinished();
                    }),
                effects.end()
            );
        }
        
        bool TextAnimator::HasActiveEffects() const {
            return std::any_of(effects.begin(), effects.end(),
                [](const std::unique_ptr<TextEffect>& effect) {
                    return effect && !effect->IsFinished();
                });
        }
        
        std::vector<CharacterInfo> TextAnimator::CalculateCharacterPositions(const std::string& text, 
                                                                             float baseX, float baseY) {
            std::vector<CharacterInfo> positions;
            positions.reserve(text.length());
            
            float currentX = baseX;
            
            for (size_t i = 0; i < text.length(); ++i) {
                char c = text[i];
                
                CharacterInfo info;
                info.character = c;
                info.x = currentX;
                info.y = baseY;
                info.index = static_cast<int>(i);
                
                // Get character dimensions from target text if available
                if (targetText) {
                    std::string singleChar(1, c);
                    info.width = targetText->GetTextWidth(singleChar);
                    info.height = targetText->GetTextHeight();
                    currentX += info.width;
                } else {
                    // Fallback: assume fixed width
                    info.width = 10.0f;
                    info.height = 16.0f;
                    currentX += info.width;
                }
                
                positions.push_back(info);
            }
            
            return positions;
        }
    }
}