#include <iostream>

#include "game_process.hpp"

using namespace ssecs;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    sdl::GameEngine *gameEngine;
    try
    {
        gameEngine = new sdl::GameEngine();
        gameEngine->Initialization();

        gameEngine->AssignWindow(
            SDL_CreateWindow("SSECS Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN));
        gameEngine->CreateProcess<GameProcess>();
        gameEngine->Run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        gameEngine->Destroy();
        return 1;
    }

    return 0;
}
