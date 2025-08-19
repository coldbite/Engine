#include "HEX.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace Engine::Graphics;

HEX::HEX(std::string value) {
    if(!value.empty() && value[0] == '#') {
        value = value.substr(1);
    }

    // #abc → #aabbcc
    if(value.size() == 3) {
        std::string expanded;

        for(char c : value) {
            expanded.push_back(c);
            expanded.push_back(c);
        }

        value = expanded;
    }

    // #abcd → #aabbccdd
    if(value.size() == 4) {
        std::string expanded;

        for(char c : value) {
            expanded.push_back(c);
            expanded.push_back(c);
        }

        value = expanded;
    }

    // RGB (6) or RGBA (8)
    if(value.size() == 6) {
        value += "ff";
    } else if(value.size() != 8) {
        throw std::invalid_argument("Ungültiges Hex-Format: " + value);
    }

    // Parse hex string in 2-character chunks (RRGGBBAA)
    unsigned int r, g, b, a;
    std::stringstream ss_r(value.substr(0, 2));
    std::stringstream ss_g(value.substr(2, 2));
    std::stringstream ss_b(value.substr(4, 2));
    std::stringstream ss_a(value.substr(6, 2));
    
    ss_r >> std::hex >> r;
    ss_g >> std::hex >> g;
    ss_b >> std::hex >> b;
    ss_a >> std::hex >> a;

    red   = r / 255.0f;
    green = g / 255.0f;
    blue  = b / 255.0f;
    alpha = a / 255.0f;
}

HEX::~HEX() = default;

float HEX::GetRed() const {
    return red;
}

float HEX::GetBlue() const {
    return blue;
}

float HEX::GetGreen() const {
    return green;
}

float HEX::GetAlpha() const {
    return alpha;
}
