/*
 * coldbite Engine
 *
 * Copyright (c) 2024. All rights reserved.
 *
 * This source code is licensed under the MIT License found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @author  Adrian Preuß
 * @version 1.0.0
 * @created 20.11.2024
 * @updated N/A
 */

module Engine.Render.Targets.OpenGL.API;

import Engine;
import std.stdio;
import core.sys.windows.windows;

extern (Windows) {
    alias GLvoid = void;
    alias GLfloat = float;
    alias GLclampf = float;
    alias GLint = int;
    alias GLuint = uint;
    alias GLenum = uint;
    int GL_COLOR_BUFFER_BIT = 0x00004000;

    void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void glClear(GLenum mask);
    void glBegin(GLenum mode);
    void glEnd();
    void glVertex2f(GLfloat x, GLfloat y);
    void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
    void glFlush();
}

class API : Renderer {
    private HDC hdc;

    override void initialize(HWND hWnd) {
        writeln("[OpenGL] initialize");

        if (hWnd is null) {
            writeln("Fehler: HWND ist null.");
            return;
        }

        hdc     = GetDC(hWnd);

        if(hdc == null) {
            writeln("Fehler beim Abrufen des Geräte-Kontexts.");
            return;
        }

        PIXELFORMATDESCRIPTOR pfd = {
            PIXELFORMATDESCRIPTOR.sizeof, // Größe der Struktur
            1,                            // Version
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
            PFD_TYPE_RGBA,                // Farbsystem
            32,                           // Farbtiefe
            0, 0, 0, 0, 0, 0,             // Farb-Bits
            0,                            // Alpha-Bits
            0,                            // Alpha-Shift
            0,                            // Accum-Bits
            0, 0, 0, 0,                   // Accum-Bits für RGBA
            24,                           // Z-Buffer-Tiefe
            8,                            // Stencil-Bits
            0,                            // Auxiliary-Buffers
            PFD_MAIN_PLANE,               // Layer-Typ
            0,                            // Reserved
            0, 0, 0                       // Masken
        };

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (pixelFormat == 0) {
            writeln("Fehler: Pixel-Format konnte nicht gewählt werden.");
            return;
        }

        if (SetPixelFormat(hdc, pixelFormat, &pfd) == 0) {
            writeln("Fehler: Pixel-Format konnte nicht gesetzt werden.");
            return;
        }

        HGLRC hglrc = wglCreateContext(hdc);
        if(hglrc is null) {
            writeln("Fehler: OpenGL-Kontext konnte nicht erstellt werden.");
            return;
        }

        if (!wglMakeCurrent(hdc, hglrc)) {
            writeln("Fehler: OpenGL-Kontext konnte nicht aktiviert werden.");
            return;
        }

        //ShowWindow(hWnd, SW_SHOW);
        //UpdateWindow(hWnd);
        writeln("GL = OK");
        SwapBuffers(hdc);
    }

    override void rendering() {
        //writeln("[OpenGL] render");

        // Erstelle einen OpenGL-Kontext (Fenstercode vorausgesetzt)

        // OpenGL-Hintergrundfarbe setzen
        glClearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange (R=1.0, G=0.5, B=0.0, Alpha=1.0)
        glClear(GL_COLOR_BUFFER_BIT);

        // Zeichne ein Dreieck
        glBegin(0x0004); // GL_TRIANGLES
        glColor3f(1.0f, 0.0f, 0.0f); // Rot
        glVertex2f(-0.5f, -0.5f);    // Links unten

        glColor3f(0.0f, 1.0f, 0.0f);
        SwapBuffers(hdc);

    }
}
