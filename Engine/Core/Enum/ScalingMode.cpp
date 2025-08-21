#include "Enum/ScalingMode.h"

namespace Engine {
    std::string_view EnumStringMap<ScalingMode>::ToString(ScalingMode value) {
        for (const auto& [enumValue, name] : values) {
            if (enumValue == value) return name;
        }
        return "UNKNOWN";
    }
    
    ScalingMode EnumStringMap<ScalingMode>::FromString(std::string_view str) {
        for (const auto& [enumValue, name] : values) {
            if (name == str) return enumValue;
        }
        return ScalingMode::FIT; // Default fallback
    }
}