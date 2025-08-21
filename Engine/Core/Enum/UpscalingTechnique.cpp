#include "Enum/UpscalingTechnique.h"

namespace Engine {
    std::string_view EnumStringMap<UpscalingTechnique>::ToString(UpscalingTechnique value) {
        for (const auto& [enumValue, name] : values) {
            if (enumValue == value) return name;
        }
        return "UNKNOWN";
    }
    
    UpscalingTechnique EnumStringMap<UpscalingTechnique>::FromString(std::string_view str) {
        for (const auto& [enumValue, name] : values) {
            if (name == str) return enumValue;
        }
        return UpscalingTechnique::LINEAR; // Default fallback
    }
}