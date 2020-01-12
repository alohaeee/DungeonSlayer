#ifndef SETTING_HPP
#define SETTING_HPP

#include <type_traits>

namespace ssecs::ecs
{

/*! @brief A class to use to push around lists of types, nothing more. */
template <typename...>
struct type_list
{
};

#define SSECS_OPAQUE_TYPE(clazz, type)                                                                                 \
    enum class clazz : type                                                                                            \
    {                                                                                                                  \
    };                                                                                                                 \
    constexpr auto to_integer(const clazz id) noexcept                                                                 \
    {                                                                                                                  \
        return std::underlying_type_t<clazz>(id);                                                                      \
    }                                                                                                                  \
    static_assert(true)

} // namespace ssecs::ecs

#endif // SETTING_HPP
