#include "../UI/Effect.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        void TextEffect::Update(float deltaTime) {
            currentTime += deltaTime;
            
            if (HasStarted() && duration >= 0.0f && loop) {
                // For looping effects, loop only the effective time (after start delay)
                float effectTime = GetEffectTime();
                if (effectTime >= duration) {
                    currentTime = startDelay + fmod(effectTime, duration);
                }
            }
        }
    }
}
