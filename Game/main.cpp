#include "Masterball.h"
#include "../Engine/Core/CommandLine/Parser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Masterball game;
    Engine::CommandLine::Parser parser;

    /* Register Arguments */
    parser.AddArgument("connect",   "c");
    parser.AddArgument("server",    "s");
    parser.AddArgument("version",   "v");
    parser.AddArgument("debug",     "d");

    /* Handle Arguments */
    if(parser.Parse(argc, argv)) {
        if(parser.HasArgument("version")) {
            std::cout << "Version: 1.0.0" << std::endl;
            return 0;
        }

        if(parser.HasArgument("debug")) {
            game.SetOption<MB, bool>(MB::DEBUGGING, true);
        }

        if(parser.HasArgument("server")) {
            std::cout << "DEDICATED SERVER" << std::endl;
            game.SetOption<MB, std::string>(MB::TYPE, "SERVER");
        } else if(parser.HasArgument("connect")) {
            std::string server = parser.GetValue("connect");
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
