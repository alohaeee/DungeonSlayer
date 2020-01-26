#ifndef CACHE_HPP
#define CACHE_HPP

#include <array>
#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <type_traits>

namespace ssecs::resource
{
/**
 * @brief Resource container.
 * @tparam Resourcs Resource type.
 * @tparam Eraser Function invokable with erasing.
 */
template <typename Resource, void (*Eraser)(Resource) = nullptr>
class cache
{

public:
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Type of resources managed by a cache. */
    using resource_type = Resource;
    /*! @brief Unique identifier type for resources. */
    using id_type = std::string;
    /*! @brief Erase function type. */
    using eraser_type = void (*)(resource_type);

    /*! @brief Default constructor. */
    cache() = default;

    /*! @brief Invoke reset */
    ~cache()
    {
        reset();
    }

    /**
     * @brief Get erase templated function.
     * @return Pointer to templated function.
     */
    const eraser_type get_eraser() const noexcept
    {
        return Eraser;
    }

    /**
     * @brief Return count of resources.
     * @return Size of iternal map.
     */
    size_type size() const noexcept
    {
        m_resources.size();
    }

    /**
     * @brief Returns true if a cache contains no resources, false otherwise.
     * @return True if the cache contains no resources, false otherwise.
     */
    bool empty() const noexcept
    {
        m_resources.empty();
    }

    /*! @brief Reset cache */
    void reset()
    {
        if constexpr (Eraser)
        {
            for (auto &[id, resource] : m_resources)
            {
                Eraser(resource);
            }
        }

        m_resources.clear();
    }

    /**
     * @brief Load instance in cache.
     * @note If such id already exist in cache, resource will replace with an instance.
     * @param id Valid id type.
     * @param instance Valid resource.
     * @return Loaded in cache resource.
     */
    resource_type load(const id_type id, resource_type instance)
    {
        if (has(id))
        {
            erase(id);
        }
        return m_resources.emplace(id, instance).first->second;
    }

    /**
     * @brief Get resource by id.
     * @warning Throw exception if such id not exist.
     * @param id Valid id type.
     * @return Resource in success.
     */
    resource_type resource(const id_type id)
    {
        return m_resources.at(id);
    }

    /**
     * @brief Checks if an id exist in cache.
     * @param id Valid id type.
     * @return True if exist, otherwise false.
     */
    bool has(const id_type id) const noexcept
    {
        return (m_resources.find(id) != m_resources.end());
    }

    /**
     * @brief Erase resource from cache.
     * @param id Valid id type.
     */
    void erase(id_type id)
    {
        if constexpr (Eraser)
        {
            auto resource = m_resources.at(id);
            Eraser(resource);
        }
        m_resources.erase(id);
    }

private:
    std::map<id_type, resource_type> m_resources;
};
} // namespace ssecs::resource

#endif // CACHE_HPP
