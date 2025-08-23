#pragma once

// Convenience header that includes all available text effects
#include "WaveEffect.h"
#include "FadeEffect.h"
#include "TypewriterEffect.h"
#include "BounceEffect.h"
#include "ScaleEffect.h"
#include "ColorCycleEffect.h"
#include "SlideEffect.h"
#include "FadeSlideEffect.h"
#include "AmbienceEffect.h"

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
            
            // New animation effects
            inline std::unique_ptr<BounceEffect> CreateBounce(float intensity = 20.0f, float frequency = 3.0f, float delay = 0.1f) {
                return std::make_unique<BounceEffect>(-1.0f, intensity, frequency, true, delay);
            }
            
            inline std::unique_ptr<ScaleEffect> CreateScale(float minScale = 0.8f, float maxScale = 1.2f, float frequency = 2.0f, float delay = 0.1f) {
                return std::make_unique<ScaleEffect>(-1.0f, minScale, maxScale, frequency, true, delay);
            }
            
            inline std::unique_ptr<ColorCycleEffect> CreateColorCycle(float frequency = 1.0f, float delay = 0.05f) {
                return std::make_unique<ColorCycleEffect>(-1.0f, frequency, true, delay);
            }
            
            inline std::unique_ptr<SlideEffect> CreateSlideFromLeft(float distance = 100.0f, float duration = 2.0f, float delay = 0.0f) {
                return std::make_unique<SlideEffect>(duration, SlideDirection::LEFT, distance, delay);
            }
            
            inline std::unique_ptr<SlideEffect> CreateSlideFromRight(float distance = 100.0f, float duration = 2.0f, float delay = 0.0f) {
                return std::make_unique<SlideEffect>(duration, SlideDirection::RIGHT, distance, delay);
            }
            
            inline std::unique_ptr<SlideEffect> CreateSlideFromTop(float distance = 100.0f, float duration = 2.0f, float delay = 0.0f) {
                return std::make_unique<SlideEffect>(duration, SlideDirection::TOP, distance, delay);
            }
            
            inline std::unique_ptr<SlideEffect> CreateSlideFromBottom(float distance = 100.0f, float duration = 2.0f, float delay = 0.0f) {
                return std::make_unique<SlideEffect>(duration, SlideDirection::BOTTOM, distance, delay);
            }
            
            // GIF-specific animations
            inline std::unique_ptr<FadeSlideEffect> CreateFadeSlideOut(float distance = 150.0f, float duration = 3.0f, float delay = 0.1f) {
                return std::make_unique<FadeSlideEffect>(duration, distance, delay, true, true); // true = fadeOut, true = loop
            }
            
            inline std::unique_ptr<FadeSlideEffect> CreateFadeSlideIn(float distance = 150.0f, float duration = 3.0f, float delay = 0.1f) {
                return std::make_unique<FadeSlideEffect>(duration, distance, delay, false, true); // false = fadeIn, true = loop
            }
            
            inline std::unique_ptr<AmbienceEffect> CreateAmbience(float frequency = 0.5f) {
                return std::make_unique<AmbienceEffect>(-1.0f, frequency, true);
            }
        }
    }
}