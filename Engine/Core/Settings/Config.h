#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <iostream>

namespace Engine {
    namespace Settings {
        using ConfigValue = std::variant<bool, int, float, std::string>;
        
        class Config {
        public:
            Config() = default;
            explicit Config(const std::string& configPath);
            ~Config() = default;
            
            static bool Load(const std::string& configPath);
            static Config& GetInstance();
            
            // Static convenience methods
            static bool Has(const std::string& key) {
                return GetInstance().HasImpl(key);
            }
            
            template<typename T>
            static T Get(const std::string& key, const T& defaultValue = T{}) {
                return GetInstance().GetImpl<T>(key, defaultValue);
            }
            
            static std::string GetString(const std::string& key, const std::string& defaultValue = "") {
                return GetInstance().GetStringImpl(key, defaultValue);
            }
            
            static int GetInt(const std::string& key, int defaultValue = 0) {
                return GetInstance().GetIntImpl(key, defaultValue);
            }
            
            static float GetFloat(const std::string& key, float defaultValue = 0.0f) {
                return GetInstance().GetFloatImpl(key, defaultValue);
            }
            
            static bool GetBool(const std::string& key, bool defaultValue = false) {
                return GetInstance().GetBoolImpl(key, defaultValue);
            }
            
            static void Set(const std::string& key, const ConfigValue& value) {
                GetInstance().SetImpl(key, value);
            }
            
            static void PrintConfig() {
                GetInstance().PrintConfigImpl();
            }
            
            static void Clear() {
                GetInstance().ClearImpl();
            }
            
            bool LoadFromFile(const std::string& configPath);
            
            // Instance methods (renamed with Impl suffix)
            bool HasImpl(const std::string& key) const;
            
            template<typename T>
            T GetImpl(const std::string& key, const T& defaultValue = T{}) const {
                auto it = values.find(key);
                if (it != values.end()) {
                    try {
                        return std::get<T>(it->second);
                    } catch (const std::bad_variant_access&) {
                        std::cout << "Config: Type mismatch for key '" << key 
                                  << "', returning default value" << std::endl;
                    }
                }
                return defaultValue;
            }
            
            std::string GetStringImpl(const std::string& key, const std::string& defaultValue = "") const;
            int GetIntImpl(const std::string& key, int defaultValue = 0) const;
            float GetFloatImpl(const std::string& key, float defaultValue = 0.0f) const;
            bool GetBoolImpl(const std::string& key, bool defaultValue = false) const;
            
            void SetImpl(const std::string& key, const ConfigValue& value);
            
            void PrintConfigImpl() const;
            void ClearImpl();
            
        private:
            std::unordered_map<std::string, ConfigValue> values;
            
            std::string ResolvePath(const std::string& configPath);
            std::string GetExecutableDirectory();
            
            bool ParseLine(const std::string& line, std::string& currentGroup);
            bool ParseKeyValue(const std::string& line, const std::string& currentGroup);
            bool ParseGroupStart(const std::string& line, std::string& currentGroup);
            bool ParseGroupEnd(const std::string& line, std::string& currentGroup);
            
            std::string Trim(const std::string& str);
            std::string RemoveQuotes(const std::string& str);
            std::string RemoveAllWhitespace(const std::string& str);
            std::string RemoveLeadingTrailingWhitespace(const std::string& str);
            ConfigValue ParseValue(const std::string& valueStr);
            bool ParseBool(const std::string& str);
            
            bool IsNumeric(const std::string& str);
            bool IsFloat(const std::string& str);
        };
    }
}
