#include "Settings/Options.h"
#include <iostream>

namespace Engine {
    namespace Settings {
        // Non-template utility functions
        void PrintOptionValue(const OptionValue& value) {
            std::visit([](const auto& val) {
                std::cout << val;
            }, value);
        }

        void PrintTypeMismatchWarning(int option) {
            std::cout << "Warning: Option type mismatch for enum value " << option << std::endl;
        }

        // OptionsManager implementation
        OptionsManager& OptionsManager::GetInstance() {
            static OptionsManager instance;
            return instance;
        }
    }
}
