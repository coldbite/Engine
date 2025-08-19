#include "OpenGL.h"
#include "../../Core/NativeWindow.h"
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

            void OpenGL::SetViewport(int width, int height) {
                glViewport(0, 0, width, height);
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
                Clear(RGBA(0.0f, 0.0f, 0.0f, 1.0f));
            }

            void OpenGL::Clear(const IColor& color) {
                if(!initialized) {
                    std::cout << "[OpenGL] Clear called but OpenGL not initialized!" << std::endl;
                    return;
                }

                glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
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

            void OpenGL::DrawRect(float x, float y, float width, float height, const IColor& color) {
                if(!initialized) {
                    return;
                }

                // Enable blending for transparency
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());

                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + width, y);
                    glVertex2f(x + width, y + height);
                    glVertex2f(x, y + height);
                glEnd();

                // Restore blending state
                glDisable(GL_BLEND);
            }

            void OpenGL::PaintText(const std::string& text, float x, float y, const IColor& color) {
                if(!initialized) {
                    std::cout << "[DEBUG] PaintText returning early - not initialized" << std::endl;
                    return;
                }

                // Enable blending for font transparency
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Set text color
                glColor3f(color.GetRed(), color.GetBlue(), color.GetGreen());

                // @ToDo color.GetAlpha()

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
        }
    }
}
