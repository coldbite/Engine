#include "Enum/Renderer.h"

namespace Engine {
    std::string_view EnumStringMap<Renderer>::ToString(Renderer value) {
        for (const auto& [enumValue, name] : values) {
            if (enumValue == value) return name;
        }
        return "Unknown";
    }
    
    Renderer EnumStringMap<Renderer>::FromString(std::string_view str) {
        for (const auto& [enumValue, name] : values) {
            if (name == str) return enumValue;
        }
        return Renderer::OPENGL; // Default fallback
    }
}