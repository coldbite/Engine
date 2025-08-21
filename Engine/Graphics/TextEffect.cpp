#include "TextEffect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        void TextEffect::Update(float deltaTime) {
            if (duration < 0.0f) {
                // Infinite duration
                currentTime += deltaTime;
            } else {
                currentTime += deltaTime;
                
                if (loop && currentTime >= duration) {
                    currentTime = fmod(currentTime, duration);
                }
            }
        }
    }
}