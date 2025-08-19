#pragma once

#include "IColor.h"
#include "RGBA.h"
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
        class IRenderingAPI;

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
            void RenderText(IRenderingAPI& renderingAPI, const std::string& text, float x, float y, const IColor& color = RGBA(1.0f, 1.0f, 1.0f, 1.0f)) const;

            void SetFontSize(unsigned int fontSize);
            unsigned int GetFontSize() const { return m_fontSize; }

            float GetTextWidth(const std::string& text, float scale = 1.0f) const;
            float GetTextHeight(float scale = 1.0f) const;

            // Schriftgröße dynamisch an Fensterhöhe anpassen (im Resize-Callback aufrufen)
            void UpdateFontSizeForWindow(int windowHeight, unsigned int baseFontSize, int referenceHeight = 720);

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
