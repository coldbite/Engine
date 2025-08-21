#pragma once

#include "EnumStringMap.h"

namespace Engine {
    enum class UpscalingTechnique {
        NEAREST,    // Nearest neighbor (pixelated)
        LINEAR,     // Linear/Bilinear filtering
        TAAU,       // Temporal Anti-Aliasing Upsample
        DLSS,       // NVIDIA DLSS (if available)
        FSR,        // AMD FSR (if available)
        XeSS       // Intel XeSS (if available)
    };

    template<>
    struct EnumStringMap<UpscalingTechnique> {
        static constexpr std::array<std::pair<UpscalingTechnique, std::string_view>, 6> values = {{
            {UpscalingTechnique::NEAREST, "NEAREST"},
            {UpscalingTechnique::LINEAR, "LINEAR"},
            {UpscalingTechnique::TAAU, "TAAU"},
            {UpscalingTechnique::DLSS, "DLSS"},
            {UpscalingTechnique::FSR, "FSR"},
            {UpscalingTechnique::XeSS, "XeSS"}
        }};
        
        static std::string_view ToString(UpscalingTechnique value);
        static UpscalingTechnique FromString(std::string_view str);
    };
}