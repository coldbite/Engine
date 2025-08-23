#include "RGBA.h"
using namespace Engine::Graphics;

RGBA::RGBA(int red, int green, int blue, int alpha) : red(red / 255.0f), green(green / 255.0f), blue(blue / 255.0f), alpha(alpha / 100.0f) {
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
