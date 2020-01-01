#ifndef FAMILY_H
#define FAMILY_H

#include <cstddef>


namespace ssecs
{
namespace ecs
{

class family {
    using id_type = std::size_t;

    static id_type identifier() noexcept
    {
        static id_type value = 0;
        return value++;
    }

public:
    template<typename>
    static id_type type() noexcept
    {
        static const id_type value = identifier();
        return value;
    }
};
}
}

#endif // FAMILY_H
