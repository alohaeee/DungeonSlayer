#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <type_traits>

#include "config.h"

namespace ssecs::ecs
{
/*! @brief Predeclaration for compiler. */
template <typename>
struct entity_traits;

/**
 * @brief Entity traits for a 16 bits entity identifier.
 *
 * A 16 bits entity identifier guarantees:
 *
 * * 8 bits for the entity number.
 * * 8 bit for the version.
 */
template <>
struct entity_traits<std::uint16_t>
{
    /*! @brief Underlying entity type. */
    using entity_type = std::uint16_t;
    /*! @brief Underlying version type. */
    using version_type = std::uint8_t;
    /*! @brief Difference type. */
    using difference_type = std::int32_t;

    /*! @brief Mask to use to get the entity number out of an identifier. */
    static constexpr std::uint16_t entity_mask = 0xFF;
    /*! @brief Mask to use to get the version out of an identifier. */
    static constexpr std::uint16_t version_mask = 0xFF;
    /*! @brief Extent of the entity number within an identifier. */
    static constexpr auto entity_shift = 8;
};

///**
// * @brief Entity traits for a 32 bits entity identifier.
// *
// * A 32 bits entity identifier guarantees:
// *
// * * 20 bits for the entity number (suitable for almost all the games).
// * * 12 bit for the version (resets in [0-4095]).
// */
// template<>
// struct entity_traits<std::uint32_t> {
//    /*! @brief Underlying entity type. */
//    using entity_type = std::uint32_t;
//    /*! @brief Underlying version type. */
//    using version_type = std::uint16_t;
//    /*! @brief Difference type. */
//    using difference_type = std::int64_t;

//    /*! @brief Mask to use to get the entity number out of an identifier. */
//    static constexpr std::uint32_t entity_mask = 0xFFFFF;
//    /*! @brief Mask to use to get the version out of an identifier. */
//    static constexpr std::uint32_t version_mask = 0xFFF;
//    /*! @brief Extent of the entity number within an identifier. */
//    static constexpr auto entity_shift = 20;
//};

namespace internal
{

class null
{
    template <typename Entity>
    using traits_type = entity_traits<std::underlying_type_t<Entity>>;

public:
    template <typename Entity>
    constexpr operator Entity() const noexcept
    {
        return Entity{traits_type<Entity>::entity_mask};
    }

    constexpr bool operator==(null) const noexcept
    {
        return true;
    }

    constexpr bool operator!=(null) const noexcept
    {
        return false;
    }

    template <typename Entity>
    constexpr bool operator==(const Entity entity) const noexcept
    {
        return (to_integer(entity) & traits_type<Entity>::entity_mask) == to_integer(static_cast<Entity>(*this));
    }

    template <typename Entity>
    constexpr bool operator!=(const Entity entity) const noexcept
    {
        return !(entity == *this);
    }
};

template <typename Entity>
constexpr bool operator==(const Entity entity, null other) noexcept
{
    return other == entity;
}

template <typename Entity>
constexpr bool operator!=(const Entity entity, null other) noexcept
{
    return other != entity;
}

} // namespace internal

/**
 * @brief Compile-time constant for null entities.
 *
 * There exist implicit conversions from this variable to entity identifiers of
 * any allowed type. Similarly, there exist comparision operators between the
 * null entity and any other entity identifier.
 */
[[maybe_unused]] constexpr auto null = internal::null{};

} // namespace ssecs::ecs

#endif // ENTITY_HPP
