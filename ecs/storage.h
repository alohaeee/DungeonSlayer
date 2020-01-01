#ifndef STORAGE_H
#define STORAGE_H

#include "sparceset.h"
#include "entity.h"

namespace ssecs
{
namespace ecs
{
    using ComponentStorage = tools::SparceSet<Entity>;
}
}

#endif // STORAGE_H
