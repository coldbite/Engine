#pragma once

#include "EnumStringMap.h"

namespace Engine {
    enum class ScalingMode {
        STRETCH,    // Stretch to fill window (may distort)
        LETTER,     // Letterboxing (black bars on top/bottom)
        PILLAR,     // Pillarboxing (black bars on sides)
        FIT,        // Fit entire content (letterbox or pillar automatically)
        CROP,       // Crop content to fill window (may cut off content)
        INTEGER     // Integer scaling (pixel-perfect)
    };

    template<>
    struct EnumStringMap<ScalingMode> {
        static constexpr std::array<std::pair<ScalingMode, std::string_view>, 6> values = {{
            {ScalingMode::STRETCH, "STRETCH"},
            {ScalingMode::LETTER, "LETTER"},
            {ScalingMode::PILLAR, "PILLAR"},
            {ScalingMode::FIT, "FIT"},
            {ScalingMode::CROP, "CROP"},
            {ScalingMode::INTEGER, "INTEGER"}
        }};
        
        static std::string_view ToString(ScalingMode value);
        static ScalingMode FromString(std::string_view str);
    };
}