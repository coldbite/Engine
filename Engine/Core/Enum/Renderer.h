#pragma once
#include "EnumStringMap.h"
#include <array>
#include <string_view>
#include <utility>

namespace Engine {
    enum class Renderer {
        OPENGL,
        VULKAN,
        DIRECTX_9,
        DIRECTX_10,
        DIRECTX_11,
        DIRECTX_12
    };

    template<>
    struct EnumStringMap<Renderer> {
        static constexpr std::array<std::pair<Renderer, std::string_view>, 6> values = {{
            {Renderer::OPENGL, "OpenGL"},
            {Renderer::VULKAN, "Vulkan"},
            {Renderer::DIRECTX_9, "DirectX 9"},
            {Renderer::DIRECTX_10, "DirectX 10"},
            {Renderer::DIRECTX_11, "DirectX 11"},
            {Renderer::DIRECTX_12, "DirectX 12"}
        }};

        static std::string_view ToString(Renderer value);
        static Renderer FromString(std::string_view str);
    };
}
