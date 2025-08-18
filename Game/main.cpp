#include "Masterball.h"
#include <iostream>

using Engine::Arguments;

int main(int argc, char* argv[]) {
    Masterball game;
    Arguments arguments;

    /* Register Arguments */
    arguments.Add("connect",   "c");
    arguments.Add("server",    "s");
    arguments.Add("version",   "v");
    arguments.Add("debug",     "d");

    /* Handle Arguments */
    if(arguments.Parse(argc, argv)) {
        if(arguments.Has("version")) {
            std::cout << "Version: 1.0.0" << std::endl;
            return 0;
        }

        if(arguments.Has("debug")) {
            game.SetOption<MB, bool>(MB::DEBUGGING, true);
        }

        if(arguments.Has("server")) {
            std::cout << "DEDICATED SERVER" << std::endl;
            game.SetOption<MB, std::string>(MB::TYPE, "SERVER");
        } else if(arguments.Has("connect")) {
            std::string server = arguments.Get("connect");
            std::cout << "Connect parameter: " << server << std::endl;
            game.SetOption<MB, std::string>(MB::TYPE, "CLIENT");
        }
    }

    /* Init Game */
    if(!game.Initialize()) {
        std::cout << "Failed to initialize game!" << std::endl;
        return -1;
    }

    /* Run */
    game.Run();

    return 0;
}
