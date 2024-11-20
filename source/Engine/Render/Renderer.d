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

module Engine.Render.Renderer;

import Engine;
import std.stdio;

enum RendererType {
    Vulkan,
    OpenGL,
    DirectX
}

class Renderer {
    this() {
        writeln("Super.Renderer");
    }

    ~this() {
        writeln("Shutdown Renderer");
    }

    abstract void initialize();
    abstract void rendering();
}
