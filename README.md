> [!CAUTION]
> This Project is **currently** under development!

# Prerequisites
You must installed the D-Language compiler from [dlang.org](https://dlang.org).

For an easy development, we recommend `Jetbrains IntelliJ IDEA` with [d-lang Plugin](https://github.com/intellij-dlanguage/intellij-dlanguage).

The Plugin works currently not on all `Jetbrains` IDE's Versions.

# Installation
### with `Git`
> git clone https://github.com/coldbite/Engine.git

Open the cloned directory with `Jetbrains IntelliJ IDEA` ([Version `2024.2`!](https://www.jetbrains.com/de-de/idea/download/other.html)) and you can start directly.

### with `D`
> dub add coldbite

# Usage
### Add the engine to your project & create an basic Example:
```d
import std.stdio;
import core.runtime;
import coldbite;
import std.stdio;

class Game : BaseGame {
	override void initialize() {
		writeln("Game initialized");

		setTitle("Example Game");
		setSize(800, 600);
		//setPosition(-1, -1);
		//setFullscreen(true);

        // [Optional] Set Renderer: Vulkan, OpenGL, DirectX
		setRenderer(RendererType.Vulkan);

		// Init your game
		start();
	}

	override void running() {
		writeln("GameLoop");
        
		// Game-Loop
	}

	override void rendering() {
		writeln("Render");
        
        // Render your things
	}
}

void main() {
	Game game = new Game();
    
    // Init your Game
	game.initialize();
}
```