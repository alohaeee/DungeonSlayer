#ifndef GAME_INSTANCE_HPP
#define GAME_INSTANCE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "config.hpp"
#include "framerate.hpp"
#include "scene.hpp"
#include "timer.hpp"

#include "events.hpp"
#include "graphics.hpp"

namespace ssecs::sdl
{

class Game
{
    friend Instances;

private:
    Game()
    {
    }
    ~Game()
    {
    }

    void InputUpdate()
    {
        SDL_PollEvent(&Events::m_event);

        // Check for terminate.
        if (Events::Event().type == SDL_QUIT)
        {
            Quit();
        }
        if (Events::Event().window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            Graphics::DestroyLayers();
            Graphics::CreateLayers();
        }

        m_scene->InputUpdate();
    }
    void Update(const float dt)
    {
        m_scene->Update(dt);
    }
    void FixedUpdate()
    {
        m_scene->FixedUpdate();
    }

    void Render()
    {
        Graphics::OnRenderStart();

        m_scene->Render();

        Graphics::DrawLayers();
        Graphics::RenderPresent();
    }

    /*! @brief Destroy window and renderer.
     * Close all subsystems. */
    void Destroy()
    {
        Graphics::DestroyData();

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

public:
    /*! @brief Initialization SDL's subsystems. */
    void InitializeSubsystems()
    {
        if (m_hadInitialization)
        {
            return;
        }
        if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || TTF_Init() == -1 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
        {
            SDL_THROW();
        }

        m_hadInitialization = true;
    }
    /**
     * @brief Returns init state.
     * @return True if initialization was invoked, false otherwise.
     */
    bool HadInitialization() const noexcept
    {
        return m_hadInitialization;
    }
    /**
     * @brief Assign the window to graphic engine and create render from it.
     * @param window Valid SDL window.
     * @param rendererFlags RendererFlags:
     * * SDL_RENDERER_SOFTWARE - the renderer is a software fallback
     * * SDL_RENDERER_ACCELERATED - the renderer uses hardware acceleration
     * * SDL_RENDERER_PRESENTVSYNC - present is synchronized with the refresh rate
     * * SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture
     */
    void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        Graphics::AssignWindow(window, rendererFlags);
    }

    /**
     * @brief Run the game loop.
     * Check whether Initialization was invoked, existence of window and process.
     * @note Calls Destroy after loop.
     */
    void Run()
    {
        m_isRunning = true;

        while (m_isRunning)
        {

            m_frameRate.OnFrameStart();

            FixedUpdate();

            // Variable-timed update:
            Update(m_frameRate.dt);

            InputUpdate();

            Render();
        }
    }
    /*! @brief Request to quit from the game loop */
    void Quit() noexcept
    {
        m_isRunning = false;
    }

    /*! @brief Return current game state. */
    bool IsRunning() const noexcept
    {
        return m_isRunning;
    }

    /**
     * @brief Return current scene.
     */
    const auto Scene() const noexcept
    {
        return m_scene.get();
    }

    /**
     * @brief Create new process.
     * @tparam Process Desired process.
     * @tparam Args Process constructor argument
     */
    template <typename Scene, typename... Args>
    auto CreateScene(Args... args)
    {
        m_scene = std::make_unique<Scene>(std::forward<Args>(args)...);
    }

private:
    bool m_hadInitialization = false;
    bool m_isRunning = false;
    FrameRate m_frameRate;
    std::unique_ptr<BasicScene> m_scene = std::make_unique<BasicScene>();
};

} // namespace ssecs::sdl

#endif // GAME_INSTANCE_HPP
