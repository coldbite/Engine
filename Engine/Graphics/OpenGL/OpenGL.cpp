#include "Graphics/OpenGL/OpenGL.h"
#include "Core/NativeWindow.h"
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <wingdi.h>
#include <vector>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Engine {
    namespace Graphics {
        namespace OpenGL {
            std::shared_ptr<NativeWindow> OpenGL::currentWindow = nullptr;
            bool OpenGL::initialized = false;

            OpenGL::~OpenGL() {

            }

            bool OpenGL::Available() {
                // Dummy Window-Klasse registrieren
                WNDCLASSA wc = {};
                wc.style = CS_OWNDC;
                wc.lpfnWndProc = DefWindowProcA;
                wc.hInstance = GetModuleHandle(nullptr);
                wc.lpszClassName = "CheckOpenGL";

                if (!RegisterClassA(&wc)) return false;

                // Dummy-Fenster erstellen (unsichtbar)
                HWND hwnd = CreateWindowA(
                    wc.lpszClassName, "CheckOpenGLWindow",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                    nullptr, nullptr, wc.hInstance, nullptr
                    );

                if (!hwnd) return false;

                HDC hdc = GetDC(hwnd);

                // Pixel-Format wählen
                PIXELFORMATDESCRIPTOR pfd = {};
                pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
                pfd.nVersion = 1;
                pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
                pfd.iPixelType = PFD_TYPE_RGBA;
                pfd.cColorBits = 32;
                pfd.cDepthBits = 24;
                pfd.cStencilBits = 8;
                pfd.iLayerType = PFD_MAIN_PLANE;

                int pf = ChoosePixelFormat(hdc, &pfd);
                if (pf == 0) return false;
                if (!SetPixelFormat(hdc, pf, &pfd)) return false;

                // OpenGL-Kontext erstellen
                HGLRC hglrc = wglCreateContext(hdc);
                if (!hglrc) return false;
                if (!wglMakeCurrent(hdc, hglrc)) return false;

                // Version prüfen
                version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

                // Aufräumen
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(hglrc);
                ReleaseDC(hwnd, hdc);
                DestroyWindow(hwnd);
                UnregisterClassA(wc.lpszClassName, wc.hInstance);

                return (!version.empty());
            }

            std::string OpenGL::GetVersion() {
                return version;
            }

            void OpenGL::SetViewport(int width, int height) {
                glViewport(0, 0, width, height);
            }

            void OpenGL::SetViewport(int x, int y, int width, int height) {
                glViewport(x, y, width, height);
            }

            int OpenGL::GetWidth() {
                return currentWindow->GetWidth();
            }

            int OpenGL::GetHeight() {
                return currentWindow->GetHeight();
            }

            bool OpenGL::Init(std::shared_ptr<NativeWindow> window) {
                if(initialized) {
                    return true;
                }

                currentWindow = window;

                glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");

                if(!glGenerateMipmap_ptr) {
                    throw std::runtime_error("glGenerateMipmap konnte nicht geladen werden!");
                }

                glGenTextures_ptr = (PFNGLGENTEXTURESPROC) wglGetProcAddress("glGenTextures");
                glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");

                if(!glGenTextures_ptr || !glGenerateMipmap_ptr) {
                    throw std::runtime_error("Konnte OpenGL-Funktionen nicht laden!");
                }

                // Basic OpenGL setup - this was moved from NativeWindow::SetupRenderingContext
                std::cout << "[OpenGL] Initializing OpenGL context" << std::endl;

                // Enable basic OpenGL features
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Set viewport to match window size
                const auto& props = window->GetProperties();
                glViewport(0, 0, props.width, props.height);

                std::cout << "[OpenGL] OpenGL initialized successfully" << std::endl;

                initialized = true;
                return true;
            }

            void OpenGL::CreateDevice() {
                // Implementation for device creation
            }

            void OpenGL::GetDevice() {
                // Implementation for getting the device
            }

            void OpenGL::CreateContext() {
                // Implementation for context creation
            }

            void OpenGL::GetContext() {
                // Implementation for getting the context
            }

            void OpenGL::Shutdown() {
                if (!initialized) {
                    return;
                }

                std::cout << "[OpenGL] Shutting down OpenGL" << std::endl;
                currentWindow = nullptr;
                initialized = false;
            }

            void OpenGL::MakeContextCurrent() {
                if (!initialized || !currentWindow) {
                    return;
                }

                currentWindow->MakeContextCurrent();
            }

            // Virtual method implementations
            void OpenGL::Clear() {
                Clear(new RGBA(0.0f, 0.0f, 0.0f, 1.0f));
            }

            void OpenGL::Clear(IColor* color) {
                if(!initialized) {
                    std::cout << "[OpenGL] Clear called but OpenGL not initialized!" << std::endl;
                    return;
                }

                glClearColor(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void OpenGL::SwapBuffers() {
                if(!initialized || !currentWindow) {
                    return;
                }

                currentWindow->SwapBuffers();
            }

            void OpenGL::Begin2D(int width, int height) {
                if(!initialized) {
                    return;
                }

                // Set viewport to match the rendering dimensions
                glViewport(0, 0, width, height);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0, width, height, 0, -1, 1); // Top-left origin for UI

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

                glDisable(GL_DEPTH_TEST);
            }

            void OpenGL::End2D() {
                if(!initialized) {
                    return;
                }

                glEnable(GL_DEPTH_TEST);
            }

            void OpenGL::DrawRect(float x, float y, float width, float height, IColor* color) {
                if(!initialized) {
                    return;
                }

                // Enable blending for transparency
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glColor4f(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());

                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + width, y);
                    glVertex2f(x + width, y + height);
                    glVertex2f(x, y + height);
                glEnd();

                // Restore blending state
                glDisable(GL_BLEND);
            }

            void OpenGL::PaintText(const std::string& text, float x, float y, IColor* color) {
                if(!initialized) {
                    std::cout << "[DEBUG] PaintText returning early - not initialized" << std::endl;
                    return;
                }

                // Enable blending for font transparency
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Set text color
                glColor3f(color->GetRed(), color->GetBlue(), color->GetGreen());

                // @ToDo color->GetAlpha()

                // Enable texturing
                glEnable(GL_TEXTURE_2D);

                // @ToDo Draw String
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            Texture OpenGL::LoadTexture(const std::string& filename) {
                Texture tex{};

                int channels;
                unsigned char* data = stbi_load(filename.c_str(), &tex.width, &tex.height, &channels, 4);

                if(!data) {
                   throw std::runtime_error("Konnte PNG nicht laden: " + filename);
                }

                glGenTextures(1, &tex.id);
                glBindTexture(GL_TEXTURE_2D, tex.id);

                // Bilddaten hochladen
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

                // Mipmap erzeugen
                glGenerateMipmap(GL_TEXTURE_2D);

                // Standard-Parameter setzen
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // Speicher freigeben (OpenGL hat jetzt die Daten)
                stbi_image_free(data);

                return tex;
            }

            void OpenGL::DrawTexture(const Texture& texture, float x, float y, float width, float height) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture.id);
                glColor3f(1.0f, 1.0f, 1.0f);

                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);
                glEnd();

                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
            }

            void OpenGL::DrawTextureBlurred(const Texture& texture, float x, float y, float width, float height, float blurRadius) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, texture.id);

                // Subtle natural blur effect like Steam screenshot
                // Step 1: Render the main image darker for better contrast with scanlines
                glColor4f(0.6f, 0.6f, 0.6f, 0.8f); // Darkened main image
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);
                glEnd();

                // Step 2: Add subtle blur layers with very low intensity
                float baseAlpha = 0.05f; // Very subtle blur layers
                int maxPasses = static_cast<int>(blurRadius * 0.5f); // Fewer passes for subtlety

                for(int pass = 1; pass <= maxPasses; pass++) {
                    float radius = pass * (blurRadius / maxPasses);
                    float alpha = baseAlpha * (1.0f - (static_cast<float>(pass-1) / maxPasses));

                    glColor4f(1.0f, 1.0f, 1.0f, alpha); // Full brightness, very low alpha

                    // Simple 4-direction blur (horizontal and vertical only)
                    // Horizontal blur
                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(x + radius, y);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width + radius, y);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width + radius, y + height);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(x + radius, y + height);
                    glEnd();

                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(x - radius, y);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width - radius, y);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width - radius, y + height);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(x - radius, y + height);
                    glEnd();

                    // Vertical blur
                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + radius);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y + radius);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height + radius);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height + radius);
                    glEnd();

                    glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y - radius);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y - radius);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height - radius);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height - radius);
                    glEnd();
                }

                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);
                glDisable(GL_TEXTURE_2D);
            }

            void OpenGL::DrawDiagonalLines(float x, float y, float width, float height, float lineSpacing, float lineWidth, IColor* color) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());

                // Draw diagonal lines from top-left to bottom-right
                glLineWidth(lineWidth);
                glBegin(GL_LINES);

                // Calculate the diagonal distance to cover entire area
                float maxDistance = width + height;

                // Draw lines at regular intervals
                for(float offset = -height; offset < width + height; offset += lineSpacing) {
                    // Start point (top edge or left edge)
                    float startX, startY;
                    float endX, endY;

                    if(offset < 0) {
                        // Line starts on left edge
                        startX = x;
                        startY = y - offset;
                    } else {
                        // Line starts on top edge
                        startX = x + offset;
                        startY = y;
                    }

                    // End point (bottom edge or right edge)
                    if(offset + height > width) {
                        // Line ends on bottom edge
                        endX = x + width - (offset + height - width);
                        endY = y + height;
                    } else {
                        // Line ends on right edge
                        endX = x + width;
                        endY = y + offset + height;
                    }

                    // Clamp to rectangle bounds
                    if(startY > y + height) continue;
                    if(endY < y) continue;
                    if(startX > x + width) continue;
                    if(endX < x) continue;

                    // Clamp start point
                    if(startY < y) {
                        float ratio = (y - startY) / (endY - startY);
                        startX = startX + ratio * (endX - startX);
                        startY = y;
                    }
                    if(startX < x) {
                        float ratio = (x - startX) / (endX - startX);
                        startY = startY + ratio * (endY - startY);
                        startX = x;
                    }

                    // Clamp end point
                    if(endY > y + height) {
                        float ratio = (y + height - startY) / (endY - startY);
                        endX = startX + ratio * (endX - startX);
                        endY = y + height;
                    }
                    if(endX > x + width) {
                        float ratio = (x + width - startX) / (endX - startX);
                        endY = startY + ratio * (endY - startY);
                        endX = x + width;
                    }

                    // Draw the line
                    glVertex2f(startX, startY);
                    glVertex2f(endX, endY);
                }

                glEnd();
                glDisable(GL_BLEND);
            }

            void OpenGL::DrawRadialLines(float x, float y, float width, float height, float centerX1, float centerY1, float centerX2, float centerY2, int numLines, float lineWidth, IColor* color) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());
                glLineWidth(lineWidth);

                glBegin(GL_LINES);

                // Draw radial lines from first center point
                for(int i = 0; i < numLines; i++) {
                    float angle = (i * 360.0f / numLines) * (3.14159f / 180.0f); // Convert to radians
                    float lineLength = width + height; // Long enough to extend beyond screen

                    float endX1 = centerX1 + cos(angle) * lineLength;
                    float endY1 = centerY1 + sin(angle) * lineLength;

                    // Clip line to screen bounds
                    float clippedStartX = centerX1;
                    float clippedStartY = centerY1;
                    float clippedEndX = endX1;
                    float clippedEndY = endY1;

                    // Simple bounds check - only draw if line intersects with screen
                    if((clippedStartX >= x && clippedStartX <= x + width && clippedStartY >= y && clippedStartY <= y + height) ||
                       (clippedEndX >= x && clippedEndX <= x + width && clippedEndY >= y && clippedEndY <= y + height)) {
                        glVertex2f(clippedStartX, clippedStartY);
                        glVertex2f(clippedEndX, clippedEndY);
                    }
                }

                // Draw radial lines from second center point
                for(int i = 0; i < numLines; i++) {
                    float angle = (i * 360.0f / numLines) * (3.14159f / 180.0f); // Convert to radians
                    float lineLength = width + height; // Long enough to extend beyond screen

                    float endX2 = centerX2 + cos(angle) * lineLength;
                    float endY2 = centerY2 + sin(angle) * lineLength;

                    // Clip line to screen bounds
                    float clippedStartX = centerX2;
                    float clippedStartY = centerY2;
                    float clippedEndX = endX2;
                    float clippedEndY = endY2;

                    // Simple bounds check - only draw if line intersects with screen
                    if((clippedStartX >= x && clippedStartX <= x + width && clippedStartY >= y && clippedStartY <= y + height) ||
                       (clippedEndX >= x && clippedEndX <= x + width && clippedEndY >= y && clippedEndY <= y + height)) {
                        glVertex2f(clippedStartX, clippedStartY);
                        glVertex2f(clippedEndX, clippedEndY);
                    }
                }

                glEnd();
                glDisable(GL_BLEND);
            }

            void OpenGL::DrawVerticalLines(float x, float y, float width, float height, float lineSpacing, float lineWidth, IColor* color) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());
                glLineWidth(lineWidth);

                glBegin(GL_LINES);

                // Draw simple vertical lines across the screen
                for(float lineX = x; lineX <= x + width; lineX += lineSpacing) {
                    glVertex2f(lineX, y);
                    glVertex2f(lineX, y + height);
                }

                glEnd();
                glDisable(GL_BLEND);
            }

            void OpenGL::DrawHorizontalLines(float x, float y, float width, float height, float lineSpacing, float lineWidth, IColor* color) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(color->GetRed(), color->GetGreen(), color->GetBlue(), color->GetAlpha());
                glLineWidth(lineWidth);

                // Use GL_QUADS for thicker, more visible lines
                glBegin(GL_QUADS);

                // Draw simple horizontal lines across the screen
                // lineSpacing should be the total distance between line starts
                // This means: lineSpacing = lineWidth + gap between lines
                for(float lineY = y; lineY <= y + height; lineY += lineSpacing + lineWidth) {
                    // Draw rectangle for each line
                    glVertex2f(x, lineY);
                    glVertex2f(x + width, lineY);
                    glVertex2f(x + width, lineY + lineWidth);
                    glVertex2f(x, lineY + lineWidth);
                }
                glEnd();
                glDisable(GL_BLEND);
            }

            void OpenGL::DrawFilmGrain(float x, float y, float width, float height, float intensity, int seed) {
                if(!initialized) {
                    return;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Simple pseudo-random number generator for film grain
                auto random = [](int& state) -> float {
                    state = (state * 1103515245 + 12345) & 0x7fffffff;
                    return (float)state / (float)0x7fffffff;
                };

                int randomState = seed + static_cast<int>(x * y); // Different seed per position

                glBegin(GL_POINTS);

                // Generate random grain points across the screen
                int grainDensity = static_cast<int>(width * height * 0.0002f); // About 0.02% of pixels

                for(int i = 0; i < grainDensity; i++) {
                    // Random position within the rectangle
                    float grainX = x + random(randomState) * width;
                    float grainY = y + random(randomState) * height;

                    // Random brightness (both bright and dark grain)
                    float brightness = random(randomState);
                    float alpha = intensity * random(randomState);

                    // Mix of bright and dark spots for authentic film grain
                    if (brightness > 0.5f) {
                        // Bright grain
                        glColor4f(1.0f, 1.0f, 1.0f, alpha);
                    } else {
                        // Dark grain
                        glColor4f(0.0f, 0.0f, 0.0f, alpha);
                    }

                    glVertex2f(grainX, grainY);
                }

                glEnd();
                glDisable(GL_BLEND);

                // Add some slightly larger grain for more authentic look
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBegin(GL_QUADS);

                int largerGrainCount = static_cast<int>(width * height * 0.00005f); // Fewer but larger grains
                randomState += 1000; // Different sequence for larger grains

                for(int i = 0; i < largerGrainCount; i++) {
                    float grainX = x + random(randomState) * width;
                    float grainY = y + random(randomState) * height;
                    float grainSize = 0.5f + random(randomState) * 1.5f; // 0.5-2.0 pixel size

                    float brightness = random(randomState);
                    float alpha = intensity * 0.5f * random(randomState);

                    if (brightness > 0.6f) {
                        glColor4f(1.0f, 1.0f, 1.0f, alpha);
                    } else {
                        glColor4f(0.0f, 0.0f, 0.0f, alpha);
                    }

                    // Draw small rectangle for grain
                    glVertex2f(grainX, grainY);
                    glVertex2f(grainX + grainSize, grainY);
                    glVertex2f(grainX + grainSize, grainY + grainSize);
                    glVertex2f(grainX, grainY + grainSize);
                }

                glEnd();
                glDisable(GL_BLEND);
            }
        }
    }
}
