#include <iostream>

#include "game_scene.hpp"

using namespace ssecs::sdl;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    auto game = Instances::CreateGame();
    try
    {

        game->InitializeSubsystems();
        game->AssignWindow(
            SDL_CreateWindow("SSECS Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN),
            SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
        game->CreateScene<GameScene>();
        game->Run();

        fontCache.reset();
        musicCache.reset();
        scoreTable.Save("score.txt");
        Instances::DestroyGame();

        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        fontCache.reset();
        Instances::DestroyGame();
        return 1;
    }
}
