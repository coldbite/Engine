// Template implementations for Options.h
// This file is included at the end of Options.h

#include <iostream>

namespace Engine {
    namespace Settings {
        
        // OptionsRegistry template implementations
        template<typename EnumType>
        OptionsRegistry<EnumType>& OptionsRegistry<EnumType>::GetInstance() {
            static OptionsRegistry<EnumType> instance;
            return instance;
        }
        
        template<typename EnumType>
        void OptionsRegistry<EnumType>::SetOption(EnumType option, const OptionValue& value) {
            options[static_cast<int>(option)] = value;
        }
        
        template<typename EnumType>
        bool OptionsRegistry<EnumType>::HasOption(EnumType option) const {
            return options.find(static_cast<int>(option)) != options.end();
        }
        
        template<typename EnumType>
        OptionValue OptionsRegistry<EnumType>::GetOptionVariant(EnumType option) const {
            auto it = options.find(static_cast<int>(option));
            if (it != options.end()) {
                return it->second;
            }
            return OptionValue{};
        }
        
        template<typename EnumType>
        void OptionsRegistry<EnumType>::RemoveOption(EnumType option) {
            options.erase(static_cast<int>(option));
        }
        
        template<typename EnumType>
        void OptionsRegistry<EnumType>::ClearOptions() {
            options.clear();
        }
        
        template<typename EnumType>
        void OptionsRegistry<EnumType>::PrintOptions() const {
            std::cout << "Options for enum type:" << std::endl;
            for (const auto& pair : options) {
                std::cout << "  Enum(" << pair.first << ") = ";
                PrintOptionValue(pair.second);
                std::cout << std::endl;
            }
        }
    }
}