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

namespace Engine {
    namespace Graphics {
        namespace OpenGL {
            std::shared_ptr<NativeWindow> OpenGL::currentWindow = nullptr;
            bool OpenGL::initialized = false;

            OpenGL::~OpenGL() {

            }

            bool OpenGL::Init(std::shared_ptr<NativeWindow> window) {
                if(initialized) {
                    return true;
                }

                currentWindow = window;

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

            void OpenGL::Clear(float r, float g, float b, float a) {
                if (!initialized) {
                    std::cout << "[OpenGL] Clear called but OpenGL not initialized!" << std::endl;
                    return;
                }

                glClearColor(r, g, b, a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void OpenGL::SwapBuffers() {
                if (!initialized || !currentWindow) {
                    return;
                }

                currentWindow->SwapBuffers();
            }

            void OpenGL::Begin2D(int width, int height) {
                if (!initialized) {
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
                if (!initialized) {
                    return;
                }

                glEnable(GL_DEPTH_TEST);
            }

            void OpenGL::DrawRect(float x, float y, float width, float height, float r, float g, float b, float a) {
                if (!initialized) {
                    return;
                }

                glColor4f(r, g, b, a);
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + width, y);
                    glVertex2f(x + width, y + height);
                    glVertex2f(x, y + height);
                glEnd();
            }

            void OpenGL::DrawText(const std::string& text, float x, float y, float r, float g, float b) {
                std::cout << "[DEBUG] DrawText called: '" << text << "' initialized=" << initialized << std::endl;

                if (!initialized) {
                    std::cout << "[DEBUG] DrawText returning early - not initialized" << std::endl;
                    return;
                }

                // Enable blending for font transparency
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Set text color
                glColor3f(r, g, b);

                // Enable texturing
                glEnable(GL_TEXTURE_2D);

                // @ToDo Draw String
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            void OpenGL::DrawSpinner(float x, float y, float radius, float rotation) {
                if (!initialized) {
                    return;
                }

                glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

                const int segments = 12;
                const float angleStep = 2.0f * 3.14159f / segments;

                for (int i = 0; i < segments; ++i) {
                    float alpha = 1.0f - (float)i / segments;
                    glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.8f);

                    float angle = rotation + i * angleStep;
                    float startX = x + cos(angle) * (radius * 0.6f);
                    float startY = y + sin(angle) * (radius * 0.6f);
                    float endX = x + cos(angle) * radius;
                    float endY = y + sin(angle) * radius;

                    glLineWidth(3.0f);
                    glBegin(GL_LINES);
                        glVertex2f(startX, startY);
                        glVertex2f(endX, endY);
                    glEnd();
                }
            }
        }
    }
}
