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

module Engine.Render.Targets.DirectX.API;

import Engine;
import std.stdio;

class API : Renderer {
    override void initialize(HWND hWnd) {
        writeln("[DirectX] initialize");
    }

    override void rendering() {
       writeln("[DirectX] render");
    }
}