#pragma once
#include "IColor.h"
#include <string>

namespace Engine {
    namespace Graphics {
        class HEX : public IColor {
            public:
                explicit HEX(std::string value);
                virtual ~HEX();

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
