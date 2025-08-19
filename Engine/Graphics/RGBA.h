#pragma once
#include "IColor.h"

namespace Engine {
    namespace Graphics {
        class RGBA : public IColor {
            public:
                RGBA(float red, float green, float blue, float alpha);
                virtual ~RGBA();

                float GetRed() const override;
                float GetBlue() const override;
                float GetGreen() const override;
                float GetAlpha() const override;

            private:
                float red   = 0.0f;
                float green = 0.0f;
                float blue  = 0.0f;
                float alpha = 1.0f;
        };
    }
}
