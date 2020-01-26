#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <cmath>
#include <string>

namespace ssecs::component
{

/**
 * @brief The Vector2D class.
 * Representation of 2D vectors and points.
 */
class Vector2D
{

public:
    using coordinate_type = float;

    /*! @brief Shorthand for writing Vector2(0, 0). */
    static constexpr const auto zero()
    {
        return Vector2D();
    }
    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr const auto up()
    {
        return Vector2D(0, 1);
    }
    /*! @brief Shorthand for writing Vector2(0, -1). */
    static constexpr const auto down()
    {
        return Vector2D(0, -1);
    }
    /*! @brief Shorthand for writing Vector2(-1, 0). */
    static constexpr const auto left()
    {
        return Vector2D(-1, 0);
    }
    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr const auto right()
    {
        return Vector2D(0, 1);
    }
    /*! @brief Shorthand for writing Vector2(1, 1). */
    static constexpr const auto one()
    {
        return Vector2D(1, 1);
    }

    /*! @brief Default constructor !*/
    constexpr Vector2D() : m_x(0), m_y(0)
    {
    }

    /*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const coordinate_type x, const coordinate_type y) : m_x(x), m_y(y)
    {
    }
    /*! @brief Copy constructor. */
    Vector2D(const Vector2D &other)
    {
        *this = other;
    }
    /*! @brief Default destructor. */
    ~Vector2D() = default;

    /*! @brief Returns x coordinate. */
    const coordinate_type &x() const noexcept
    {
        return m_x;
    }

    /*! @brief Returns y coordinate. */
    const coordinate_type &y() const noexcept
    {
        return m_y;
    }

    /*! @brief Compute vector lenght and returns it. */
    const coordinate_type magnitude() const noexcept
    {
        return std::sqrt(m_x * m_x + m_y * m_y);
    }

    /*! @brief Returns vector with lenght 1. */
    const Vector2D normalized() const noexcept
    {
        auto length = magnitude();
        return Vector2D(m_x / length, m_y / length);
    }

    /*! @brief Self normalize. */
    void Normalize()
    {
        *this = normalized();
    }

    /*! @brief Set x and y coordinates. */
    void Set(const coordinate_type x, const coordinate_type y) noexcept
    {
        m_x = x;
        m_y = y;
    }

    /*! @brief Check if two vectors are equals. */
    bool Equals(const Vector2D &lhs, const Vector2D &rhs) const noexcept
    {
        return lhs == rhs;
    }

    /*! @brief Returns formated coordinates in string. */
    std::string ToString() const noexcept
    {
        return "x: " + std::to_string(m_x) + "\t" + "y:" + std::to_string(m_y);
    }

    /*! @brief Copy assgment. */
    Vector2D &operator=(const Vector2D &other)
    {
        if (this != &other)
        {
            this->m_x = other.m_x;
            this->m_y = other.m_y;
        }

        return *this;
    }
    /*! @brief Multiplys vector by number. */
    Vector2D operator*(coordinate_type multiplier) const noexcept
    {
        Vector2D other(m_x * multiplier, m_y * multiplier);
        return other;
    }
    /*! @brief Divides vector by number. */
    Vector2D operator/(coordinate_type divider) const
    {
        Vector2D other(m_x / divider, m_y / divider);
        return other;
    }

    /*! @brief Adds to vectors. */
    Vector2D operator+(const Vector2D &other) const noexcept
    {
        Vector2D result(m_x + other.m_x, m_y + other.m_y);
        return result;
    }

    /*! @brief Subtracts one vector from another. */
    Vector2D operator-(const Vector2D &other) const noexcept
    {
        Vector2D result(m_x - other.m_x, m_y - other.m_y);
        return result;
    }

    /*! @brief Returns true if two vectors are approximately equal. */
    bool operator==(const Vector2D &other) const noexcept
    {
        return (m_x == other.m_x && m_y == other.m_y);
    }

    /*! @brief Returns false if two vectors are approximately equal. */
    bool operator!=(const Vector2D &other) const noexcept
    {
        return !(*this == other);
    }

    static Vector2D Lerp(const Vector2D &a, const Vector2D &b, float t)
    {
        return b * t + a * (1 - t);
    }

private:
    coordinate_type m_x;
    coordinate_type m_y;
};
} // namespace ssecs::component

#endif // VECTOR2D_HPP
