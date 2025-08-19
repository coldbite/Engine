#include "RGB.h"
using namespace Engine::Graphics;

RGB::RGB(float red, float green, float blue) : red(red), green(green), blue(blue) {
    /* Do Nothing */
}

float RGB::GetRed() const {
    return red;
}

float RGB::GetBlue() const {
    return blue;
}

float RGB::GetGreen() const {
    return green;
}

float RGB::GetAlpha() const {
    return 1.0f;
}
