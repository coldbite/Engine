#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <linux/limits.h>
#endif

namespace Engine {
    namespace Settings {
        Config::Config(const std::string& configPath) {
            LoadFromFile(configPath);
        }
        
        bool Config::Load(const std::string& configPath) {
            Config& instance = GetInstance();
            return instance.LoadFromFile(configPath);
        }
        
        Config& Config::GetInstance() {
            static Config instance;
            return instance;
        }
        
        bool Config::LoadFromFile(const std::string& configPath) {
            std::string fullPath = ResolvePath(configPath);
            std::ifstream file(fullPath);
            
            if (!file.is_open()) {
                //std::cout << "Config: Failed to open file: " << fullPath << std::endl;
                return false;
            }
            
            Clear();
            std::string line;
            std::string currentGroup = "";
            int lineNumber = 0;
            
            while (std::getline(file, line)) {
                lineNumber++;
                if (!ParseLine(line, currentGroup)) {
                    //std::cout << "Config: Parse error at line " << lineNumber << ": " << line << std::endl;
                }
            }
            
            file.close();
            return true;
        }
        
        bool Config::HasImpl(const std::string& key) const {
            return values.find(key) != values.end();
        }
        
        std::string Config::GetStringImpl(const std::string& key, const std::string& defaultValue) const {
            return GetImpl<std::string>(key, defaultValue);
        }
        
        int Config::GetIntImpl(const std::string& key, int defaultValue) const {
            return GetImpl<int>(key, defaultValue);
        }
        
        float Config::GetFloatImpl(const std::string& key, float defaultValue) const {
            return GetImpl<float>(key, defaultValue);
        }
        
        bool Config::GetBoolImpl(const std::string& key, bool defaultValue) const {
            return GetImpl<bool>(key, defaultValue);
        }
        
        void Config::SetImpl(const std::string& key, const ConfigValue& value) {
            values[key] = value;
        }
        
        void Config::PrintConfigImpl() const {
            std::cout << "Config values:" << std::endl;
            for (const auto& pair : values) {
                std::cout << "  " << pair.first << " = ";
                std::visit([](const auto& value) {
                    std::cout << value;
                }, pair.second);
                std::cout << std::endl;
            }
        }
        
        void Config::ClearImpl() {
            values.clear();
        }
        
        std::string Config::ResolvePath(const std::string& configPath) {
            std::filesystem::path path(configPath);
            
            if (path.is_absolute()) {
                return configPath;
            }
            
            std::string exeDir = GetExecutableDirectory();
            std::filesystem::path fullPath = std::filesystem::path(exeDir) / path;
            return fullPath.string();
        }
        
        std::string Config::GetExecutableDirectory() {
#ifdef _WIN32
            char buffer[MAX_PATH];
            GetModuleFileNameA(NULL, buffer, MAX_PATH);
            std::string exePath(buffer);
            size_t pos = exePath.find_last_of("\\/");
            return (pos != std::string::npos) ? exePath.substr(0, pos) : "";
#else
            char buffer[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (len != -1) {
                buffer[len] = '\0';
                std::string exePath(buffer);
                size_t pos = exePath.find_last_of("/");
                return (pos != std::string::npos) ? exePath.substr(0, pos) : "";
            }
            return "";
#endif
        }
        
        bool Config::ParseLine(const std::string& line, std::string& currentGroup) {
            std::string trimmed = Trim(line);
            
            if (trimmed.empty() || trimmed[0] == '#') {
                return true;
            }
            
            if (ParseGroupStart(trimmed, currentGroup)) {
                return true;
            }
            
            if (ParseGroupEnd(trimmed, currentGroup)) {
                return true;
            }
            
            return ParseKeyValue(trimmed, currentGroup);
        }
        
        bool Config::ParseKeyValue(const std::string& line, const std::string& currentGroup) {
            size_t pos = line.find('=');
            if (pos == std::string::npos) {
                return false;
            }
            
            std::string key = Trim(line.substr(0, pos));
            std::string valueStr = Trim(line.substr(pos + 1));
            
            if (!currentGroup.empty()) {
                key = currentGroup + "." + key;
            }
            
            ConfigValue value = ParseValue(valueStr);
            values[key] = value;
            
            return true;
        }
        
        bool Config::ParseGroupStart(const std::string& line, std::string& currentGroup) {
            size_t pos = line.find('{');
            if (pos == std::string::npos) {
                return false;
            }
            
            std::string groupName = Trim(line.substr(0, pos));
            if (!currentGroup.empty()) {
                currentGroup = currentGroup + "." + groupName;
            } else {
                currentGroup = groupName;
            }
            
            return true;
        }
        
        bool Config::ParseGroupEnd(const std::string& line, std::string& currentGroup) {
            if (line == "}") {
                size_t pos = currentGroup.find_last_of('.');
                if (pos != std::string::npos) {
                    currentGroup = currentGroup.substr(0, pos);
                } else {
                    currentGroup.clear();
                }
                return true;
            }
            return false;
        }
        
        std::string Config::Trim(const std::string& str) {
            size_t start = str.find_first_not_of(" \t\r\n");
            if (start == std::string::npos) {
                return "";
            }
            
            size_t end = str.find_last_not_of(" \t\r\n");
            return str.substr(start, end - start + 1);
        }
        
        std::string Config::RemoveQuotes(const std::string& str) {
            if (str.length() >= 2 && 
                ((str.front() == '"' && str.back() == '"') || 
                 (str.front() == '\'' && str.back() == '\''))) {
                return str.substr(1, str.length() - 2);
            }
            return str;
        }
        
        ConfigValue Config::ParseValue(const std::string& valueStr) {
            std::string cleanValue = RemoveQuotes(valueStr);
            
            if (ParseBool(cleanValue)) {
                return true;
            }
            
            std::string lowerValue = cleanValue;
            std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
            if (lowerValue == "false" || lowerValue == "0" || lowerValue == "off") {
                return false;
            }
            
            if (IsFloat(cleanValue)) {
                return std::stof(cleanValue);
            }
            
            if (IsNumeric(cleanValue)) {
                return std::stoi(cleanValue);
            }
            
            return cleanValue;
        }
        
        bool Config::ParseBool(const std::string& str) {
            std::string lowerStr = str;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            
            return lowerStr == "true" || lowerStr == "1" || lowerStr == "on";
        }
        
        bool Config::IsNumeric(const std::string& str) {
            if (str.empty()) return false;
            
            size_t start = 0;
            if (str[0] == '-' || str[0] == '+') {
                start = 1;
                if (str.length() == 1) return false;
            }
            
            for (size_t i = start; i < str.length(); ++i) {
                if (!std::isdigit(str[i])) {
                    return false;
                }
            }
            
            return true;
        }
        
        bool Config::IsFloat(const std::string& str) {
            if (str.empty()) return false;
            
            bool hasDecimal = false;
            size_t start = 0;
            
            if (str[0] == '-' || str[0] == '+') {
                start = 1;
                if (str.length() == 1) return false;
            }
            
            for (size_t i = start; i < str.length(); ++i) {
                if (str[i] == '.') {
                    if (hasDecimal) return false;
                    hasDecimal = true;
                } else if (!std::isdigit(str[i])) {
                    return false;
                }
            }
            
            return hasDecimal;
        }
    }
}
