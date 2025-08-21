#pragma once

// Convenience header that includes all available text effects
#include "WaveEffect.h"
#include "FadeEffect.h"
#include "TypewriterEffect.h"

namespace Engine {
    namespace Graphics {
        namespace Effects {
            // Factory functions for easy effect creation
            inline std::unique_ptr<WaveEffect> CreateWave(float intensity = 15.0f, float frequency = 2.0f) {
                return std::make_unique<WaveEffect>(-1.0f, intensity, frequency, true);
            }
            
            inline std::unique_ptr<FadeInEffect> CreateFadeIn(float duration = 2.0f) {
                return std::make_unique<FadeInEffect>(duration);
            }
            
            inline std::unique_ptr<FadeOutEffect> CreateFadeOut(float duration = 2.0f) {
                return std::make_unique<FadeOutEffect>(duration);
            }
            
            inline std::unique_ptr<TypewriterEffect> CreateTypewriter(float charsPerSecond = 10.0f, bool showCursor = true) {
                return std::make_unique<TypewriterEffect>(charsPerSecond, showCursor);
            }
        }
    }
}