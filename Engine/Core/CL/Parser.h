#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace Engine {
    namespace CL {
        struct Argument {
            std::string name;
            std::string shortName;
            std::string value;
            bool hasValue;
            bool isPresent;
            
            Argument(const std::string& n, const std::string& s = "") : name(n), shortName(s), hasValue(false), isPresent(false) {}
        };

        class Parser {
        public:
            Parser();
            ~Parser();
            
            void AddArgument(const std::string& name, const std::string& shortName = "");
            bool Parse(int argc, char* argv[]);
            
            bool HasArgument(const std::string& name) const;
            std::string GetValue(const std::string& name) const;
            std::optional<std::string> GetOptionalValue(const std::string& name) const;
            
            void PrintHelp() const;
            void PrintArguments() const;
            
        private:
            std::vector<Argument> arguments;
            std::unordered_map<std::string, size_t> nameToIndex;
            std::unordered_map<std::string, size_t> shortNameToIndex;
            
            bool IsFlag(const std::string& arg) const;
            std::string StripPrefix(const std::string& arg) const;
            std::pair<std::string, std::string> SplitAssignment(const std::string& arg) const;
            size_t FindArgumentIndex(const std::string& name) const;
        };
    }
}
