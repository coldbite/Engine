#include "Text.h"
#include "IRenderingAPI.h"
#include "IColor.h"
#include "../Core/Engine.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace Engine {
    namespace Graphics {
        // Define TextAlignment static constants
        const TextAlignment TextAlignment::TOP_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::TOP);
        const TextAlignment TextAlignment::TOP_CENTER(HorizontalAlignment::CENTER, VerticalAlignment::TOP);
        const TextAlignment TextAlignment::TOP_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::TOP);
        const TextAlignment TextAlignment::CENTER_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::CENTER);
        const TextAlignment TextAlignment::CENTER(HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
        const TextAlignment TextAlignment::CENTER_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::CENTER);
        const TextAlignment TextAlignment::BOTTOM_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::BOTTOM);
        const TextAlignment TextAlignment::BOTTOM_CENTER(HorizontalAlignment::CENTER, VerticalAlignment::BOTTOM);
        const TextAlignment TextAlignment::BOTTOM_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::BOTTOM);

        bool Text::s_initialized = false;
        FT_Library Text::s_library;
        int Text::s_instanceCount = 0;

        Text::Text() : m_face(nullptr), m_fontSize(14), m_texturesGenerated(false),
                       m_text(""), m_fontName(""), m_textColor(1.0f, 1.0f, 1.0f, 1.0f), 
                       m_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f),
                       m_paddingTop(0.0f), m_paddingRight(0.0f), m_paddingBottom(0.0f), m_paddingLeft(0.0f),
                       m_marginTop(0.0f), m_marginRight(0.0f), m_marginBottom(0.0f), m_marginLeft(0.0f),
                       m_size(14.0f), m_style(FontStyle::NORMAL), m_hasBackground(false) {
            s_instanceCount++;

            if (!s_initialized) {
                if (FT_Init_FreeType(&s_library)) {
                    throw std::runtime_error("Could not initialize FreeType Library");
                }
                s_initialized = true;
            }
        }

        Text::~Text() {
            CleanupCharacters();

            if (m_face) {
                FT_Done_Face(m_face);
                m_face = nullptr;
            }

            s_instanceCount--;
            if (s_instanceCount == 0 && s_initialized) {
                FT_Done_FreeType(s_library);
                s_initialized = false;
            }
        }

        bool Text::LoadFont(const std::string& fontPath, unsigned int fontSize) {
            if (m_face) {
                FT_Done_Face(m_face);
                m_face = nullptr;
                CleanupCharacters();
            }

            if (FT_New_Face(s_library, fontPath.c_str(), 0, &m_face)) {
                std::cerr << "Failed to load font: " << fontPath << std::endl;
                return false;
            }

            m_fontSize = fontSize;
            m_fontPath = fontPath;
            m_texturesGenerated = false;  // Mark textures as not generated yet - they will be generated on first render
            FT_Set_Pixel_Sizes(m_face, 0, fontSize);

            return true;
        }

        void Text::GenerateCharacterTextures() const {
            // CleanupCharacters(); // Skip cleanup for now in const function

            // Check OpenGL errors first
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "[DEBUG] OpenGL error before texture creation: " << error << std::endl;
            }

            // Create a simple test texture that should definitely work
            unsigned int testTexture = 0;
            glGenTextures(1, &testTexture);

            error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "[DEBUG] OpenGL error after glGenTextures: " << error << std::endl;
            }

            if (testTexture == 0) {
                std::cout << "[DEBUG] ERROR: glGenTextures returned 0! OpenGL context might not be current." << std::endl;
                return;
            }

            glBindTexture(GL_TEXTURE_2D, testTexture);

            error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "[DEBUG] OpenGL error after glBindTexture: " << error << std::endl;
            }

            // 4x4 checkerboard pattern in RGB
            unsigned char testData[4 * 4 * 3] = {
                255,0,0,   0,255,0,   255,0,0,   0,255,0,
                0,255,0,   255,0,0,   0,255,0,   255,0,0,
                255,0,0,   0,255,0,   255,0,0,   0,255,0,
                0,255,0,   255,0,0,   0,255,0,   255,0,0
            };

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, testData);

            error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "[DEBUG] OpenGL error after glTexImage2D: " << error << std::endl;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            std::cout << "[DEBUG] Test texture setup complete, ID: " << testTexture << std::endl;

            // Now create real FreeType textures for each character
            for (unsigned char c = 32; c < 127; c++) {
                if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
                    continue;
                }

                FT_GlyphSlot glyph = m_face->glyph;
                FT_Bitmap& bitmap = glyph->bitmap;

                unsigned int charTexture;
                glGenTextures(1, &charTexture);
                glBindTexture(GL_TEXTURE_2D, charTexture);

                // Empty character (like space) - create 1x1 transparent texture
                if (bitmap.width == 0 || bitmap.rows == 0) {
                    unsigned char emptyData[4] = {255, 255, 255, 0};
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, emptyData);

                // Flip the bitmap data Y-axis before creating texture (FreeType liefert oben links, OpenGL erwartet unten links)
                } else {
                    unsigned char* flipped_buffer = new unsigned char[bitmap.width * bitmap.rows];
                    for (int y = 0; y < bitmap.rows; y++) {
                        for (int x = 0; x < bitmap.width; x++) {
                            int src_index = y * bitmap.width + x;
                            int dst_index = (bitmap.rows - 1 - y) * bitmap.width + x;
                            flipped_buffer[dst_index] = bitmap.buffer[src_index];
                        }
                    }

                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, bitmap.width, bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, flipped_buffer);

                    delete[] flipped_buffer;
                }

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

                Character character = {
                    charTexture,  // Use individual texture for each character
                    bitmap.width,
                    bitmap.rows,
                    glyph->bitmap_left,
                    glyph->bitmap_top,
                    static_cast<unsigned int>(glyph->advance.x >> 6)
                };
                m_characters[c] = character;
            }

            glBindTexture(GL_TEXTURE_2D, 0);
        }


        void Text::SetFontSize(unsigned int fontSize) {
            if (m_fontSize != fontSize && m_face) {
                m_fontSize = fontSize;
                FT_Set_Pixel_Sizes(m_face, 0, fontSize);
                m_texturesGenerated = false;  // Mark for regeneration on next render
            }
        }

        float Text::GetTextWidth(const std::string& text, float scale) const {
            float width = 0;
            for (char c : text) {
                auto it = m_characters.find(c);
                if (it != m_characters.end()) {
                    width += it->second.advance * scale;
                }
            }
            return width;
        }

        float Text::GetTextHeight(float scale) const {
            return m_fontSize * scale;
        }

        float Text::GetActualTextHeight(const std::string& text, float scale) const {
            if (!m_texturesGenerated) {
                GenerateCharacterTextures();
            }

            int maxBearingY = 0;
            int minBearingY = 0;
            
            // Find the actual top and bottom bounds of the text
            for (char c : text) {
                auto it = m_characters.find(c);
                if (it != m_characters.end()) {
                    const Character& ch = it->second;
                    
                    // Top of character (bearingY is distance from baseline to top)
                    if (ch.bearingY > maxBearingY) {
                        maxBearingY = ch.bearingY;
                    }
                    
                    // Bottom of character (height - bearingY gives distance from baseline to bottom)
                    int bottomY = static_cast<int>(ch.height) - ch.bearingY;
                    if (bottomY > minBearingY) {
                        minBearingY = bottomY;
                    }
                }
            }
            
            // Total height is distance from top to bottom
            return static_cast<float>(maxBearingY + minBearingY) * scale;
        }

        void Text::CleanupCharacters() {
            for (auto& pair : m_characters) {
                glDeleteTextures(1, &pair.second.textureID);
            }
            m_characters.clear();
        }

        void Text::UpdateFontSizeForWindow(int windowHeight, unsigned int baseFontSize, int referenceHeight) {
            if (referenceHeight <= 0) {
                referenceHeight = 720;
            }

            float fontScale                 = static_cast<float>(windowHeight) / static_cast<float>(referenceHeight);
            unsigned int dynamicFontSize    = static_cast<unsigned int>(baseFontSize * fontScale);

            if(dynamicFontSize < 8) {
                dynamicFontSize = 8;
            }

            if(dynamicFontSize != m_fontSize && m_face) {
                SetFontSize(dynamicFontSize);
            }
        }

        // New API method implementations
        void Text::SetValue(const std::string& text) {
            m_text = text;
        }

        void Text::SetFont(const std::string& fontName) {
            m_fontName = fontName;
            LoadFontByName(fontName);
        }

        void Text::SetColor(const IColor& color) {
            m_textColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
        }

        void Text::SetBackground(const IColor& color) {
            m_backgroundColor = RGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
            m_hasBackground = true;
        }

        void Text::SetPadding(float x, float y) {
            m_paddingLeft = m_paddingRight = x;
            m_paddingTop = m_paddingBottom = y;
        }

        void Text::SetPadding(float top, float right, float bottom, float left) {
            m_paddingTop = top;
            m_paddingRight = right;
            m_paddingBottom = bottom;
            m_paddingLeft = left;
        }

        void Text::SetMargin(float x, float y) {
            m_marginLeft = m_marginRight = x;
            m_marginTop = m_marginBottom = y;
        }

        void Text::SetMargin(float top, float right, float bottom, float left) {
            m_marginTop = top;
            m_marginRight = right;
            m_marginBottom = bottom;
            m_marginLeft = left;
        }

        void Text::SetSize(float size) {
            m_size = size;
            if (m_face) {
                SetFontSize(static_cast<unsigned int>(size));
            }
        }

        void Text::SetStyle(FontStyle style) {
            m_style = style;
        }

        void Text::Render(IRenderingAPI& context, float x, float y) {
            if (m_text.empty()) {
                return;
            }
            
            if (!m_face) {
                return;
            }

            // Apply text transformations (like UPPERCASE)
            std::string renderedText = ApplyTextTransformation(m_text);

            float renderX = x + m_marginLeft;
            float renderY = y + m_marginTop;

            // Render background if needed
            if (m_hasBackground) {
                // Ensure character textures are generated before accessing characters
                if (!m_texturesGenerated && m_face) {
                    GenerateCharacterTextures();
                    m_texturesGenerated = true;
                }
                
                float textWidth = GetTextWidth(renderedText);
                float textHeight = GetTextHeight(); // Use font size based height
                
                // Find the actual leftmost position of the text (considering bearingX)
                float minBearingX = 0;
                if (!m_characters.empty() && !renderedText.empty()) {
                    char firstChar = renderedText[0];
                    auto it = m_characters.find(firstChar);
                    if (it != m_characters.end()) {
                        minBearingX = static_cast<float>(it->second.bearingX);
                    }
                }
                
                float actualTextX = renderX + m_paddingLeft + minBearingX;
                float bgX = actualTextX - m_paddingLeft;
                float bgY = renderY - m_paddingTop;
                float bgWidth = textWidth + m_paddingLeft + m_paddingRight;
                float bgHeight = textHeight + m_paddingTop + m_paddingBottom;


                // Simple background rendering using OpenGL quads
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                glColor4f(m_backgroundColor.GetRed(), m_backgroundColor.GetGreen(), 
                         m_backgroundColor.GetBlue(), m_backgroundColor.GetAlpha());
                
                glBegin(GL_QUADS);
                    glVertex2f(bgX, bgY);
                    glVertex2f(bgX + bgWidth, bgY);
                    glVertex2f(bgX + bgWidth, bgY + bgHeight);
                    glVertex2f(bgX, bgY + bgHeight);
                glEnd();
                
                glDisable(GL_BLEND);
            }

            // Render text with padding offset
            float textX = renderX + m_paddingLeft;
            float textY = renderY + m_paddingTop;
            
            // Inline text rendering (replaces RenderText call)
            if(!m_texturesGenerated && m_face) {
                GenerateCharacterTextures();
                m_texturesGenerated = true;
            }

            if(!m_characters.empty() && !renderedText.empty()) {
                // Use the current coordinate system directly (no matrix changes)
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_TEXTURE_2D);

                // Set color
                glColor4f(m_textColor.GetRed(), m_textColor.GetGreen(), m_textColor.GetBlue(), m_textColor.GetAlpha());

                float scale = 1.0f; // Direct 1:1 pixel mapping
                float posX = textX;
                
                // Calculate maxBearingY for baseline alignment
                float maxBearingY = 0;
                for(char c : renderedText) {
                    auto it = m_characters.find(c);
                    if(it != m_characters.end()) {
                        const Character& ch = it->second;
                        if(ch.bearingY > maxBearingY) {
                            maxBearingY = static_cast<float>(ch.bearingY);
                        }
                    }
                }
                
                // Render characters
                for(char c : renderedText) {
                    auto it = m_characters.find(c);
                    if(it == m_characters.end()) {
                        continue;
                    }

                    const Character& ch = it->second;

                    float xpos = posX + ch.bearingX * scale;
                    float ypos = textY + (maxBearingY - ch.bearingY) * scale;
                    float w = ch.width * scale;
                    float h = ch.height * scale;

                    if (w > 0 && h > 0) {
                        glBindTexture(GL_TEXTURE_2D, ch.textureID);

                        glBegin(GL_QUADS);
                            glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos);
                            glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos);
                            glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos + h);
                            glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos + h);
                        glEnd();
                    }

                    posX += ch.advance * scale;
                }

                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);
                glDisable(GL_TEXTURE_2D);
            }
        }

        void Text::Render(IRenderingAPI& context, const TextAlignment& alignment) {
            if (m_text.empty() || !m_face) {
                return;
            }

            // Apply text transformations (like UPPERCASE)
            std::string renderedText = ApplyTextTransformation(m_text);

            // Calculate text dimensions including padding and margin
            float textWidth = GetTextWidth(renderedText);
            float textHeight = GetTextHeight();
            float totalWidth = textWidth + m_paddingLeft + m_paddingRight + m_marginLeft + m_marginRight;
            float totalHeight = textHeight + m_paddingTop + m_paddingBottom + m_marginTop + m_marginBottom;

            // Use reference resolution for alignment calculations
            const float REFERENCE_WIDTH = 1280.0f;
            const float REFERENCE_HEIGHT = 720.0f;

            // Calculate X position based on horizontal alignment (in reference coordinates)
            float x = 0.0f;
            switch (alignment.horizontal) {
                case HorizontalAlignment::LEFT:
                    x = 0.0f;
                    break;
                case HorizontalAlignment::CENTER:
                    x = (REFERENCE_WIDTH - totalWidth) / 2.0f;
                    break;
                case HorizontalAlignment::RIGHT:
                    x = REFERENCE_WIDTH - totalWidth;
                    break;
            }

            // Calculate Y position based on vertical alignment (in reference coordinates)
            float y = 0.0f;
            switch (alignment.vertical) {
                case VerticalAlignment::TOP:
                    y = 0.0f;
                    break;
                case VerticalAlignment::CENTER:
                    y = (REFERENCE_HEIGHT - totalHeight) / 2.0f;
                    break;
                case VerticalAlignment::BOTTOM:
                    y = REFERENCE_HEIGHT - totalHeight;
                    break;
            }

            // Use the position-based render method (which will apply scaling)
            Render(context, x, y);
        }


        void Text::LoadFontByName(const std::string& fontName) {
            Engine& engine = Engine::GetInstance();
            std::string fontPath = engine.GetFont(fontName);
            
            if (!fontPath.empty()) {
                LoadFont(fontPath, static_cast<unsigned int>(m_size));
            } else {
                std::cerr << "Font not found: " << fontName << std::endl;
            }
        }

        std::string Text::ApplyTextTransformation(const std::string& text) const {
            std::string transformedText = text;
            
            // Apply UPPERCASE transformation if style contains UPPERCASE
            if (static_cast<int>(m_style) & static_cast<int>(FontStyle::UPPERCASE)) {
                std::transform(transformedText.begin(), transformedText.end(), transformedText.begin(), ::toupper);
            }
            // Apply LOWERCASE transformation if style contains LOWERCASE
            else if (static_cast<int>(m_style) & static_cast<int>(FontStyle::LOWERCASE)) {
                std::transform(transformedText.begin(), transformedText.end(), transformedText.begin(), ::tolower);
            }
            
            return transformedText;
        }
    }
}
