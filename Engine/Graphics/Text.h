#pragma once

#include <string>
#include <memory>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _WIN32
    #include <windows.h>
    #include <wingdi.h>
    #include <gl/gl.h>
#endif

namespace Engine {
    namespace Graphics {
        
        struct Character {
            unsigned int textureID;
            unsigned int width, height;
            int bearingX, bearingY;
            unsigned int advance;
        };

        class Text {
        public:
            Text();
            ~Text();

            bool LoadFont(const std::string& fontPath, unsigned int fontSize = 48);
            void RenderText(const std::string& text, float x, float y, float scale = 1.0f, 
                          float r = 1.0f, float g = 1.0f, float b = 1.0f) const;
            
            // Test function to render simple colored rectangles instead of text
            void RenderTestText(const std::string& text, float x, float y, float scale = 1.0f, 
                               float r = 1.0f, float g = 1.0f, float b = 1.0f) const;
            
            void SetFontSize(unsigned int fontSize);
            unsigned int GetFontSize() const { return m_fontSize; }
            
            float GetTextWidth(const std::string& text, float scale = 1.0f) const;
            float GetTextHeight(float scale = 1.0f) const;

        private:
            void GenerateCharacterTextures() const;  // Made const for lazy loading
            void CleanupCharacters();

            static bool s_initialized;
            static FT_Library s_library;
            static int s_instanceCount;

            FT_Face m_face;
            unsigned int m_fontSize;
            std::string m_fontPath;  // Store font path for lazy loading
            mutable std::map<char, Character> m_characters;  // Made mutable
            mutable bool m_texturesGenerated;  // Track if textures are generated
        };
    }
}