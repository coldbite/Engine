#include "Arguments.h"
#include <iostream>

namespace Engine {
    namespace CommandLine {
        Arguments::Arguments() {}

        Arguments::~Arguments() {}

        void Arguments::Add(const std::string& name, const std::string& shortName) {
            size_t index        = arguments.size();
            arguments.emplace_back(name, shortName);
            nameToIndex[name]   = index;

            if(!shortName.empty()) {
                shortNameToIndex[shortName] = index;
            }
        }

        bool Arguments::Parse(int argc, char* argv[]) {
            for(int i = 1; i < argc; ++i) {
                std::string arg = argv[i];

                if(!IsFlag(arg)) {
                    std::cout << "Warning: Unknown argument: " << arg << std::endl;
                    continue;
                }

                auto [flagName, assignedValue]  = SplitAssignment(arg);
                std::string cleanFlag           = StripPrefix(flagName);
                size_t argIndex                 = FindArgumentIndex(cleanFlag);

                if(argIndex == SIZE_MAX) {
                    std::cout << "Warning: Unknown flag: " << arg << std::endl;
                    continue;
                }

                arguments[argIndex].isPresent = true;

                if(!assignedValue.empty()) {
                    arguments[argIndex].value       = assignedValue;
                    arguments[argIndex].hasValue    = true;
                } else if(i + 1 < argc && !IsFlag(argv[i + 1])) {
                    arguments[argIndex].value       = argv[i + 1];
                    arguments[argIndex].hasValue    = true;
                    ++i;
                }
            }

            return true;
        }

        bool Arguments::Has(const std::string& name) const {
            size_t index = FindArgumentIndex(name);
            return index != SIZE_MAX && arguments[index].isPresent;
        }

        std::string Arguments::Get(const std::string& name) const {
            size_t index = FindArgumentIndex(name);

            if(index != SIZE_MAX && arguments[index].hasValue) {
                return arguments[index].value;
            }

            return "";
        }

        std::optional<std::string> Arguments::Optional(const std::string& name) const {
            size_t index = FindArgumentIndex(name);

            if(index != SIZE_MAX && arguments[index].hasValue) {
                return arguments[index].value;
            }

            return std::nullopt;
        }

        void Arguments::PrintHelp() const {
            std::cout << "Available arguments:" << std::endl;

            for(const auto& arg : arguments) {
                std::cout << "  ";

                if(!arg.shortName.empty()) {
                    std::cout << "-" << arg.shortName << ", ";
                }

                std::cout << "--" << arg.name << std::endl;
            }
        }

        void Arguments::PrintArguments() const {
            std::cout << "Parsed arguments:" << std::endl;

            for(const auto& arg : arguments) {
                if (arg.isPresent) {
                    std::cout << "  " << arg.name;

                    if(arg.hasValue) {
                        std::cout << " = " << arg.value;
                    }

                    std::cout << std::endl;
                }
            }
        }

        bool Arguments::IsFlag(const std::string& arg) const {
            return arg.length() > 1 && (arg[0] == '-' || arg[0] == '/');
        }

        std::string Arguments::StripPrefix(const std::string& arg) const {
            if(arg.length() < 2) {
                return arg;
            }

            if(arg[0] == '/' || arg[0] == '-') {
                if(arg.length() > 2 && arg[1] == '-') {
                    return arg.substr(2);
                }

                return arg.substr(1);
            }

            return arg;
        }

        std::pair<std::string, std::string> Arguments::SplitAssignment(const std::string& arg) const {
            size_t position = arg.find('=');

            if(position != std::string::npos) {
                std::string flag    = arg.substr(0, position);
                std::string value   = arg.substr(position + 1);

                if(value.length() >= 2 && value[0] == '"' && value.back() == '"') {
                    value = value.substr(1, value.length() - 2);
                }

                return { flag, value };
            }

            return { arg, "" };
        }

        size_t Arguments::FindArgumentIndex(const std::string& name) const {
            auto nameIt = nameToIndex.find(name);

            if(nameIt != nameToIndex.end()) {
                return nameIt->second;
            }

            auto shortIt = shortNameToIndex.find(name);

            if(shortIt != shortNameToIndex.end()) {
                return shortIt->second;
            }

            return SIZE_MAX;
        }
    }
}
