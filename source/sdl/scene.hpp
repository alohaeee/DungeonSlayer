#ifndef LOOP_PROCESS_HPP
#define LOOP_PROCESS_HPP

namespace ssecs::sdl
{

/**
 * @brief The Process class.
 * Base process deliver interface for inheritance.
 * Override methods of this class to make own game logic.
 * Functions invoked in this way:
 * * FixedUpdate();
 * * Update();
 * * InputUpdate();
 * * Render();
 */
class BasicScene
{
public:
    virtual ~BasicScene() = default;

    virtual void FixedUpdate()
    {
    }
    virtual void Update([[maybe_unused]] const float dt)
    {
    }
    virtual void InputUpdate()
    {
    }
    virtual void Render()
    {
    }
};
} // namespace ssecs::sdl

#endif // LOOP_PROCESS_HPP
