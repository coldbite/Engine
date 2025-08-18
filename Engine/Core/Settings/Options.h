#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <typeindex>
#include <type_traits>

namespace Engine {
    namespace Settings {
        using OptionValue = std::variant<bool, int, float, std::string>;
        
        // Forward declarations
        void PrintOptionValue(const OptionValue& value);
        void PrintTypeMismatchWarning(int option);
        
        template<typename EnumType>
        class OptionsRegistry {
            static_assert(std::is_enum_v<EnumType>, "Template parameter must be an enum type");
            
        public:
            static OptionsRegistry<EnumType>& GetInstance();
            
            void SetOption(EnumType option, const OptionValue& value);
            
            template<typename T>
            void SetOption(EnumType option, const T& value) {
                options[static_cast<int>(option)] = OptionValue(value);
            }
            
            bool HasOption(EnumType option) const;
            
            template<typename T>
            T GetOption(EnumType option, const T& defaultValue = T{}) const {
                auto it = options.find(static_cast<int>(option));
                if (it != options.end()) {
                    try {
                        return std::get<T>(it->second);
                    } catch (const std::bad_variant_access&) {
                        PrintTypeMismatchWarning(static_cast<int>(option));
                    }
                }
                return defaultValue;
            }
            
            OptionValue GetOptionVariant(EnumType option) const;
            void RemoveOption(EnumType option);
            void ClearOptions();
            void PrintOptions() const;
            
        private:
            std::unordered_map<int, OptionValue> options;
            OptionsRegistry() = default;
        };
        
        class OptionsManager {
        public:
            static OptionsManager& GetInstance();
            
            template<typename EnumType>
            void AddOptions() {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                registryTypes[std::type_index(typeid(EnumType))] = &registry;
            }
            
            template<typename EnumType>
            void SetOption(EnumType option, const OptionValue& value) {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                registry.SetOption(option, value);
            }
            
            template<typename EnumType, typename T>
            void SetOption(EnumType option, const T& value) {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                registry.SetOption(option, value);
            }
            
            template<typename EnumType>
            bool HasOption(EnumType option) const {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                return registry.HasOption(option);
            }
            
            template<typename EnumType, typename T>
            T GetOption(EnumType option, const T& defaultValue = T{}) const {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                return registry.template GetOption<T>(option, defaultValue);
            }
            
            template<typename EnumType>
            void PrintOptions() const {
                auto& registry = OptionsRegistry<EnumType>::GetInstance();
                registry.PrintOptions();
            }
            
        private:
            std::unordered_map<std::type_index, void*> registryTypes;
            OptionsManager() = default;
        };
        
    }
}

// Include template implementations
#include "Options.inl"