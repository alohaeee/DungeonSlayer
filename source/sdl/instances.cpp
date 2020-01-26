#include "instances.hpp"

#include "game.hpp"

ssecs::sdl::Game *ssecs::sdl::Instances::GetGameInstance()
{
    return m_gameInstance;
}

ssecs::sdl::Game *ssecs::sdl::Instances::CreateGame()
{
    if (!m_gameInstance)
    {
        m_gameInstance = new Game();
    }
    return m_gameInstance;
}

void ssecs::sdl::Instances::DestroyGame()
{

    if (m_gameInstance)
    {
        m_gameInstance->Destroy();
    }
}
