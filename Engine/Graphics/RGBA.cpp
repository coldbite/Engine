#include "RGBA.h"
using namespace Engine::Graphics;

RGBA::RGBA(float red, float green, float blue, float alpha) : red(red), green(green), blue(blue), alpha(alpha) {
    /* Do Nothing */
}

RGBA::~RGBA() {
    /* Do Nothing */
}

float RGBA::GetRed() const {
    return red;
}

float RGBA::GetBlue() const {
    return blue;
}

float RGBA::GetGreen() const {
    return green;
}

float RGBA::GetAlpha() const {
    return alpha;
}
