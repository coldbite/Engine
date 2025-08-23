#pragma once
#include <string_view>
#include <iostream>
#include <type_traits>

namespace Engine {
    template<typename E>
    struct EnumStringMap;

    // SFINAE helper to detect if EnumStringMap is specialized for a type
    template<typename E, typename = void>
    struct has_enum_string_map : std::false_type {};

    template<typename E>
    struct has_enum_string_map<E, std::void_t<decltype(EnumStringMap<E>::values)>> : std::true_type {};

    template<typename E>
    std::string_view ToString(E value) {
        return EnumStringMap<E>::ToString(value);
    }

    template<typename E>
    E FromString(std::string_view str) {
        return EnumStringMap<E>::FromString(str);
    }
}

// Global stream operator - only for Engine enums that have EnumStringMap specialization
template<typename E>
std::enable_if_t<std::is_enum_v<E> && Engine::has_enum_string_map<E>::value, std::ostream&>
operator<<(std::ostream& os, E value) {
    return os << Engine::ToString(value);
}
