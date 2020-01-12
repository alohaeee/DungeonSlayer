#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "config.hpp"
#include "core_data.hpp"
#include "loop_process.hpp"
#include "timer.hpp"

namespace ssecs::sdl
{

/*! @brief Game engine manager. */
class GameEngine
{
private:
    void InputUpdate()
    {
        SDL_PollEvent(&CoreData::s_event);

        // Check for terminate.
        if (CoreData::s_event.type == SDL_QUIT)
        {
            Quit();
        }

        m_process->InputUpdate();
    }
    void Update(const float dt)
    {
        m_process->Update(dt);
    }
    void FixedUpdate()
    {
        m_process->FixedUpdate();
    }

    void Render()
    {
        SDL_RenderClear(CoreData::s_renderer);
        m_process->Render();
        SDL_RenderPresent(CoreData::s_renderer);
    }

    void Valid()
    {
        if (!m_hadInitialization)
        {
            FAST_THROW("Require to initializate SDL SubSystems!");
        }
        if (!CoreData::s_window)
        {
            auto msg = SDL_GetError();
            if (msg)
            {
                FAST_THROW(msg);
            }
            else
            {
                FAST_THROW("Window was't assigned");
            }
        }
    }

public:
    /*! @note Single tone pattern !*/
    GameEngine()
    {
        if (s_instance)
        {
            FAST_THROW("Try to create one more instance of GameEngine");
        }
        s_instance = this;
    }
    /*! @copydoc game_engine !*/
    ~GameEngine()
    {
        s_instance = nullptr;
    }

    /**
     * @brief Assign the window to game engine.
     * @param window Valid SDL window.
     * @param rendererFlags RendererFlags:
     * * SDL_RENDERER_SOFTWARE - the renderer is a software fallback
     * * SDL_RENDERER_ACCELERATED - the renderer uses hardware acceleration
     * * SDL_RENDERER_PRESENTVSYNC - present is synchronized with the refresh rate
     * * SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture
     */
    void AssignWindow(SDL_Window *window, const Uint32 rendererFlags = 0)
    {
        CoreData::s_window = window;
        Valid();

        CoreData::s_renderer = SDL_CreateRenderer(CoreData::s_window, -1, rendererFlags);
        if (!CoreData::s_renderer)
        {
            SDL_THROW();
        }
    }

    /*! @brief Initialization SDL's subsystems. */
    void Initialization()
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

    /*! @brief Run the game loop. */
    /**
     * @brief Run the game loop.
     * Check whether Initialization was invoked, existence of window and process.
     * @note Calls Destroy after loop.
     */
    void Run()
    {
        Valid();
        if (!m_process)
        {
            FAST_THROW("Process had't assigned.")
        }

        m_isRunning = true;

        m_process->Initialization();

        while (m_isRunning)
        {
            m_deltaTime = static_cast<float>(m_timer.Ellapsed() / 1000.f);
            m_timer.Record();

            const float dt = m_hasFrameLock ? (1.0f / m_targetFrameRate) : m_deltaTime;

            //            if (m_isFrameSkip)
            //            {
            //                m_deltaTimeAccumulator += dt;
            //                if (m_deltaTimeAccumulator > 10)
            //                {
            //                    // application probably lost control, fixed update would take too long
            //                    m_deltaTimeAccumulator = 0;
            //                }

            //                const float targetFrameRateInv = 1.0f / m_targetFrameRate;
            //                while (m_deltaTimeAccumulator >= targetFrameRateInv)
            //                {
            //                    FixedUpdate();
            //                    m_deltaTimeAccumulator -= targetFrameRateInv;
            //                }
            //            }
            //                        else
            //                        {

            FixedUpdate();

            // Variable-timed update:
            Update(dt);

            InputUpdate();

            Render();
        }

        Destroy();
    }

    /*! @brief Destroy window and renderer.
     * Close all subsystems. */
    void Destroy()
    {
        SDL_DestroyRenderer(CoreData::s_renderer);
        SDL_DestroyWindow(CoreData::s_window);

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    /*! @brief Request to quit from the game loop */
    void Quit() noexcept
    {
        m_isRunning = false;
    }

    /**
     * @brief Set frame lock.
     * Use TargetFrameRate to know current fps lock.
     * @param frameLock Boolean state.
     */
    void SetFrameLock(bool frameLock) noexcept
    {
        m_hasFrameLock = frameLock;
    }

    /**
     * @brief Check current lock state.
     * @return return True if frame lock is active, otherwise false.
     */
    bool HasFrameLock() const noexcept
    {
        return m_hasFrameLock;
    }

    /**
     * @brief Set target frame rate.
     * By default it's equals 60 fps.
     * @param targetFrameRate Desired frame rate.
     */
    void SetTargetFrameRate(float targetFrameRate) noexcept
    {
        m_targetFrameRate = targetFrameRate;
    }

    /**
     * @brief Return current lock frame rate.
     * @return Target frame rate.
     */
    float TargetFrameRate() const noexcept
    {
        return m_targetFrameRate;
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
     * @brief Return current process.
     */
    const auto Process() const noexcept
    {
        return m_process.get();
    }

    /**
     * @brief Create new process.
     * @tparam Process Desired process.
     * @tparam Args Process constructor argument
     */
    template <typename Process, typename... Args>
    auto CreateProcess(Args... args)
    {
        m_process = std::make_unique<Process>(std::forward<Args>(args)...);
    }

private:
    inline static GameEngine *s_instance = nullptr;
    bool m_hadInitialization = false;

    std::unique_ptr<LoopProcess> m_process;

    bool m_isRunning = false;

    //    float m_deltaTimeAccumulator = 0;
    //    bool m_isFrameSkip = false;

    float m_targetFrameRate = 60;
    bool m_hasFrameLock = false;
    float m_deltaTime = 0;

    Timer m_timer;
};

} // namespace ssecs::sdl

#endif // GAMEENGINE_H
