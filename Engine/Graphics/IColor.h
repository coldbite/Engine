#pragma once

namespace Engine {
    namespace Graphics {
        class IColor {
        public:
            virtual ~IColor() {}
            virtual float GetRed() const = 0;
            virtual float GetGreen() const = 0;
            virtual float GetBlue() const = 0;
            virtual float GetAlpha() const = 0;
        };
    }
}
