#include "Text.h"
#include "IRenderingAPI.h"
#include "IColor.h"
#include <iostream>
#include <stdexcept>

namespace Engine {
    namespace Graphics {

        bool Text::s_initialized = false;
        FT_Library Text::s_library;
        int Text::s_instanceCount = 0;

        Text::Text() : m_face(nullptr), m_fontSize(14), m_texturesGenerated(false) {
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
            m_texturesGenerated = false;  // Mark textures as not generated yet
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

       void Text::RenderTextLegacy(IRenderingAPI& context, const std::string& text, float x, float y, const IColor &color) const {
            if(!m_texturesGenerated && m_face) {
                GenerateCharacterTextures();
                m_texturesGenerated = true;
            }

            if(m_characters.empty()) {
                return;
            }

            // --- Setze Projektionsmatrix pixelgenau, Y-Achse von oben nach unten (0,0 = oben links) ---
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            //glOrtho(0.0, context.GetWidth(), context.GetHeight(), 0.0, -1.0, 1.0);
            const float REF_WIDTH = 1280.0f;
            const float REF_HEIGHT = 720.0f;
            float aspect = (float)context.GetWidth() / (float)context.GetHeight();

            // Fenster ist breiter als Referenz: Breite anpassen (pillarbox)
            float viewWidth = context.GetHeight() * aspect;
            float xOffset = (context.GetWidth() - viewWidth) / 2.0f;
            glOrtho(xOffset, xOffset + viewWidth, context.GetHeight(), 0.0, -1.0, 1.0);
            // --- Optimiertes Letterboxing/Pillarboxing: Immer im festen Referenzsystem rendern ---

            float windowWidth = static_cast<float>(context.GetWidth());
            float windowHeight = static_cast<float>(context.GetHeight());
            float windowAspect = windowWidth / windowHeight;
            float refAspect = REF_WIDTH / REF_HEIGHT;

            // Passe Viewport an, damit Seitenverhältnis stimmt (schwarze Balken möglich)
            if (windowAspect > refAspect) {
                // Pillarbox: links/rechts Balken
                int viewWidth = static_cast<int>(windowHeight * refAspect);
                int xOffset = static_cast<int>((windowWidth - viewWidth) / 2.0f);
                glViewport(xOffset, 0, viewWidth, static_cast<int>(windowHeight));
            } else {
                // Letterbox: oben/unten Balken
                int viewHeight = static_cast<int>(windowWidth / refAspect);
                int yOffset = static_cast<int>((windowHeight - viewHeight) / 2.0f);
                glViewport(0, yOffset, static_cast<int>(windowWidth), viewHeight);
            }

            // Setze Ortho-Projektion immer auf Referenzsystem
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, REF_WIDTH, REF_HEIGHT, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            // Fenster ist höher als Referenz: Höhe anpassen (letterbox)
            float viewHeight = context.GetWidth() / aspect;
            float yOffset = (context.GetHeight() - viewHeight) / 2.0f;
            glOrtho(0.0, context.GetWidth(), yOffset + viewHeight, yOffset, -1.0, 1.0);


            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glColor3f(color.GetRed(), color.GetGreen(), color.GetBlue());

            float scaleRef = 1280.0f;
            float scaleX = context.GetWidth() / scaleRef;
            float scaleY = context.GetHeight() / scaleRef;
            float scale = 1.0f; //std::max(scaleX, scaleY);
            float posX = x;

            // 1. Finde maximale bearingY für die Baseline
            int maxBearingY = 0;
            for(char c : text) {
                auto it = m_characters.find(c);
                if(it != m_characters.end()) {
                    if(it->second.bearingY > maxBearingY) maxBearingY = it->second.bearingY;
                }
            }

            // 2. Zeichne alle Zeichen an gemeinsamer Baseline
            for(char c : text) {
                auto it = m_characters.find(c);
                if(it == m_characters.end()) {
                    continue;
                }

                const Character& ch = it->second;

                float xpos = posX + ch.bearingX * scale;
                // y ist oben, maxBearingY ist die gemeinsame Baseline
                float ypos = y + (maxBearingY - ch.bearingY) * scale;
                float w = ch.width * scale;
                float h = ch.height * scale;

                float drawY = ypos;

                if (w > 0 && h > 0) {
                    glBindTexture(GL_TEXTURE_2D, ch.textureID);

                    glBegin(GL_QUADS);
                        // Vertikale Texturkoordinaten gespiegelt, damit Zeichen nicht mehr auf dem Kopf stehen
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, drawY);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, drawY);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, drawY + h);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, drawY + h);
                    glEnd();
                }

                posX += ch.advance * scale;
            }

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

            // --- Matrix wiederherstellen ---
            glEnable(GL_DEPTH_TEST);
            // Hinweis: Bei Fenster-Resize muss context.GetWidth()/GetHeight() aktuell sein und glViewport(0,0,w,h) aufgerufen werden!
        }

        void Text::RenderText(IRenderingAPI& context, const std::string& text, float x, float y, const IColor &color) const {
            if(!m_texturesGenerated && m_face) {
                GenerateCharacterTextures();
                m_texturesGenerated = true;
            }

            if(m_characters.empty()) {
                return;
            }

            // Use the current coordinate system directly (no matrix changes)
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);

            // Set color
            glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());

            // Copy the EXACT working logic from the original RenderText method
            float scale = 1.0f; // Direct 1:1 pixel mapping
            float posX = x;
            
            // Calculate maxBearingY for baseline alignment (like original)
            float maxBearingY = 0;
            for(char c : text) {
                auto it = m_characters.find(c);
                if(it != m_characters.end()) {
                    const Character& ch = it->second;
                    if(ch.bearingY > maxBearingY) {
                        maxBearingY = static_cast<float>(ch.bearingY);
                    }
                }
            }
            
            // Render characters using EXACT original logic
            for(char c : text) {
                auto it = m_characters.find(c);
                if(it == m_characters.end()) {
                    continue;
                }

                const Character& ch = it->second;

                float xpos = posX + ch.bearingX * scale;
                float ypos = y + (maxBearingY - ch.bearingY) * scale;
                float w = ch.width * scale;
                float h = ch.height * scale;

                if (w > 0 && h > 0) {
                    glBindTexture(GL_TEXTURE_2D, ch.textureID);

                    glBegin(GL_QUADS);
                        // Use EXACT original texture coordinates
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

        void Text::SetFontSize(unsigned int fontSize) {
            if (m_fontSize != fontSize && m_face) {
                m_fontSize = fontSize;
                FT_Set_Pixel_Sizes(m_face, 0, fontSize);
                GenerateCharacterTextures();
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
    }
}
