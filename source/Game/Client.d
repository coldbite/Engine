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

module Game.Client;

import Engine;
import std.stdio;

class Client : BaseGame {
    override void initialize() {
        writeln("Game initialized");

        setTitle("Basic Game");
        setSize(1024, 768);
        setPosition(-1, -1); // @ToDo Center on Screen?
        // @ToDo Resizable, Minimize, Maximize, Restore?
        //setFullscreen(true);

        setRenderer(RendererType.DirectX);

        // Init
        start();
    }

    // @ToDo uninitalize/dispose?

    override void running() {
        writeln("GameLoop");
        // Game-Loop
    }

    override void rendering() { // @ToDO Context?
        writeln("Render");
    }
}