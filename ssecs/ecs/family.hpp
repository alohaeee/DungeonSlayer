#ifndef FAMILY_HPP
#define FAMILY_HPP

#include "config.h"
#include "entity.hpp"
#include <atomic>

namespace ssecs::ecs
{
template <typename...>
class family
{
    inline static std::atomic<SSECS_ID_TYPE> identifier{};

public:
    /*! @brief Unsigned integer type. */
    using family_type = SSECS_ID_TYPE;

    /*! @brief Statically generated unique identifier for the given type. */
    template <typename... Type>
    inline static const family_type type = identifier++;
};

// class family
//{
//    inline static std::atomic<SSECS_ID_TYPE> s_identifier{};

//    static id_type identifier() noexcept
//    {
//        static id_type value = 0;
//        return value++;
//    }

// public:
//    using family_type = std::size_t;
//    template<typename>
//    static id_type type() noexcept
//    {
//        static const id_type value = identifier();
//        return value;
//    }
//};
} // namespace ssecs::ecs

#endif // FAMILY_HPP
