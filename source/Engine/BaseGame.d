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

module Engine.BaseGame;

import Engine;
import std.stdio;
import core.thread;

abstract class BaseGame {
    WindowManager window;
    Renderer renderer;

    this() {
        window = new WindowManager();
    }

    ~this() {
        writeln("Shutdown BaseGame");
    }

    // Setzt den Renderer basierend auf dem gewählten Grafik-Typ
    public void setRenderer(RendererType type) {
        switch (type) {
            case RendererType.Vulkan:
                renderer = new Vulkan();
                break;
            case RendererType.OpenGL:
                renderer = new OpenGL();
                break;
            case RendererType.DirectX:
                renderer = new DirectX();
                break;
            default:
            break;
        }
    }

    public void setTitle(string title) {
        window.setTitle(title);
    }

    public void setSize(int width, int height) {
        window.setSize(width, height);
    }

    public void setPosition(int x, int y) {
        window.setPosition(x, y);
    }

    public void setFullscreen(bool state) {
        window.setFullscreen(state);
    }

    public void start() {
        window.show(() {
            new Thread(&internalRun).start();
        });
    }

    private void internalRun() {
        bool inited = false;
        while(window.isActive()) {
            if(!inited) {
                HWND hWnd = window.getContext();

                if(hWnd !is null) {
                    inited = true;
                    renderer.initialize(hWnd);
                }
            }

            running();              // @ToDo < Client calc process
            rendering();            // @ToDo < Client render process
            renderer.rendering();// @ToDo < Renderer
        }
    }

    /* Public API */
    abstract void initialize();
    abstract void running();
    abstract void rendering(); // @ToDo Context?
}
