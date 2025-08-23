#pragma once

#include "../../IColor.h"
#include "../../RGBA.h"
#include "../Alignment.h"
#include "../Animator.h"
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

        struct Padding {
            float left;
            float right;
            float top;
            float bottom;
        };

        enum class FontStyle {
            NORMAL      = 0,
            ITALIC      = 1,
            BOLD        = 2,
            UNDERLINED  = 4,
            UPPERCASE   = 8,
            LOWERCASE   = 16
        };

        DEFINE_ENUM_FLAG_OPERATORS(FontStyle)

        struct TextAlignment {
            HorizontalAlignment horizontal;
            VerticalAlignment vertical;

            TextAlignment(HorizontalAlignment h = HorizontalAlignment::LEFT,
                         VerticalAlignment v = VerticalAlignment::TOP)
                : horizontal(h), vertical(v) {}

            // Common alignment presets
            static const TextAlignment TOP_LEFT;
            static const TextAlignment TOP_CENTER;
            static const TextAlignment TOP_RIGHT;
            static const TextAlignment CENTER_LEFT;
            static const TextAlignment CENTER;
            static const TextAlignment CENTER_RIGHT;
            static const TextAlignment BOTTOM_LEFT;
            static const TextAlignment BOTTOM_CENTER;
            static const TextAlignment BOTTOM_RIGHT;
        };

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

            // New API methods
            void SetValue(const std::string& text);
            void SetFont(const std::string& fontName);
            void SetColor(IColor* color);
            void SetBackground(IColor* color);
            void SetBackgroundEnabled(bool enabled);
            void SetPadding(float x, float y);
            void SetPadding(float top, float right, float bottom, float left);
            void SetMargin(float x, float y);
            void SetMargin(float top, float right, float bottom, float left);
            void SetSize(float size);
            void SetStyle(FontStyle style);
            virtual void Render(IRenderingAPI& context, float x, float y);
            virtual void Render(IRenderingAPI& context, const TextAlignment& alignment);

            // Animation system
            virtual void Update(float deltaTime);
            TextAnimator& GetAnimator() { return animator; }

            // Legacy methods (keep for compatibility)
            bool LoadFont(const std::string& fontPath, unsigned int fontSize = 48);

            void SetFontSize(unsigned int fontSize);
            unsigned int GetFontSize() const { return m_fontSize; }

            float GetTextWidth(const std::string& text, float scale = 1.0f) const;
            float GetTextHeight(float scale = 1.0f) const;
            Padding GetPadding() const {
                return { m_paddingLeft, m_paddingRight, m_paddingTop, m_paddingBottom };
            }

            // Get actual rendered text dimensions (more accurate for UI layout)
            float GetActualTextHeight(const std::string& text, float scale = 1.0f) const;

            // Schriftgröße dynamisch an Fensterhöhe anpassen (im Resize-Callback aufrufen)
            void UpdateFontSizeForWindow(int windowHeight, unsigned int baseFontSize, int referenceHeight = 720);

        private:
            void GenerateCharacterTextures() const;  // Made const for lazy loading
            void CleanupCharacters();
            void LoadFontByName(const std::string& fontName);
            std::string ApplyTextTransformation(const std::string& text) const;

            static bool s_initialized;
            static FT_Library s_library;
            static int s_instanceCount;

            // Legacy font loading
            FT_Face m_face;
            unsigned int m_fontSize;
            std::string m_fontPath;  // Store font path for lazy loading
            mutable std::map<char, Character> m_characters;  // Made mutable
            mutable bool m_texturesGenerated;  // Track if textures are generated

            // New text properties
            std::string m_text;
            std::string m_fontName;
            IColor* m_textColor;
            IColor* m_backgroundColor;
            float m_paddingTop, m_paddingRight, m_paddingBottom, m_paddingLeft;
            float m_marginTop, m_marginRight, m_marginBottom, m_marginLeft;
            float m_size;
            FontStyle m_style;
            bool m_hasBackground;

            // Animation system
            TextAnimator animator;
        };
    }
}
