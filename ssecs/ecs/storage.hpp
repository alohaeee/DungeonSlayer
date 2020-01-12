#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#include "config.h"
#include "sparse_set.hpp"

namespace ssecs::ecs
{

template <typename Entity, typename Type, typename = std::void_t<>>
class basic_storage : public sparse_set<Entity>
{
    using underlying_type = sparse_set<Entity>;
    using traits_type = entity_traits<std::underlying_type_t<Entity>>;

    template <bool Const>
    class iterator
    {
        friend class basic_storage<Entity, Type>;

        using instance_type = std::conditional_t<Const, const std::vector<Type>, std::vector<Type>>;
        using index_type = typename traits_type::difference_type;

        iterator(instance_type *ref, const index_type idx) noexcept : m_instances{ref}, index{idx}
        {
        }

    public:
        using difference_type = index_type;
        using value_type = Type;
        using pointer = std::conditional_t<Const, const value_type *, value_type *>;
        using reference = std::conditional_t<Const, const value_type &, value_type &>;
        using iterator_category = std::random_access_iterator_tag;

        iterator() noexcept = default;

        iterator &operator++() noexcept
        {
            return --index, *this;
        }

        iterator operator++(int) noexcept
        {
            iterator orig = *this;
            return ++(*this), orig;
        }

        iterator &operator--() noexcept
        {
            return ++index, *this;
        }

        iterator operator--(int) noexcept
        {
            iterator orig = *this;
            return --(*this), orig;
        }

        iterator &operator+=(const difference_type value) noexcept
        {
            index -= value;
            return *this;
        }

        iterator operator+(const difference_type value) const noexcept
        {
            return iterator{m_instances, index - value};
        }

        iterator &operator-=(const difference_type value) noexcept
        {
            return (*this += -value);
        }

        iterator operator-(const difference_type value) const noexcept
        {
            return (*this + -value);
        }

        difference_type operator-(const iterator &other) const noexcept
        {
            return other.index - index;
        }

        reference operator[](const difference_type value) const noexcept
        {
            const auto pos = size_type(index - value - 1);
            return (*m_instances)[pos];
        }

        bool operator==(const iterator &other) const noexcept
        {
            return other.index == index;
        }

        bool operator!=(const iterator &other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const iterator &other) const noexcept
        {
            return index > other.index;
        }

        bool operator>(const iterator &other) const noexcept
        {
            return index < other.index;
        }

        bool operator<=(const iterator &other) const noexcept
        {
            return !(*this > other);
        }

        bool operator>=(const iterator &other) const noexcept
        {
            return !(*this < other);
        }

        pointer operator->() const noexcept
        {
            const auto pos = size_type(index - 1);
            return &(*m_instances)[pos];
        }

        reference operator*() const noexcept
        {
            return *operator->();
        }

    private:
        instance_type *m_instances;
        index_type index;
    };

public:
    /*! @brief Type of the objects associated with the entities. */
    using object_type = Type;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Random access iterator type. */
    using iterator_type = iterator<false>;
    /*! @brief Constant random access iterator type. */
    using const_iterator_type = iterator<true>;

    /**
     * @brief Increases the capacity of a storage.
     *
     * If the new capacity is greater than the current capacity, new storage is
     * allocated, otherwise the method does nothing.
     *
     * @param cap Desired capacity.
     */
    void reserve(const size_type cap)
    {
        underlying_type::reserve(cap);
        m_instances.reserve(cap);
    }

    /*! @brief Requests the removal of unused capacity. */
    void shrink_to_fit()
    {
        underlying_type::shrink_to_fit();
        m_instances.shrink_to_fit();
    }

    /**
     * @brief Direct access to the array of objects.
     *
     * The returned pointer is such that range `[raw(), raw() + size()]` is
     * always a valid range, even if the container is empty.
     *
     * @note
     * There are no guarantees on the order, even though either `sort` or
     * `respect` has been previously invoked. Internal data structures arrange
     * elements to maximize performance. Accessing them directly gives a
     * performance boost but less guarantees. Use `begin` and `end` if you want
     * to iterate the storage in the expected order.
     *
     * @return A pointer to the array of objects.
     */
    const object_type *raw() const noexcept
    {
        return m_instances.data();
    }

    /*! @copydoc raw */
    object_type *raw() noexcept
    {
        return const_cast<object_type *>(std::as_const(*this).raw());
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * The returned iterator points to the first instance of the given type. If
     * the storage is empty, the returned iterator will be equal to `end()`.
     *
     * @note
     * Random access iterators stay true to the order imposed by a call to
     * either `sort` or `respect`.
     *
     * @return An iterator to the first instance of the given type.
     */
    const_iterator_type cbegin() const noexcept
    {
        const typename traits_type::difference_type pos = underlying_type::size();
        return const_iterator_type{&m_instances, pos};
    }

    /*! @copydoc cbegin */
    const_iterator_type begin() const noexcept
    {
        return cbegin();
    }

    /*! @copydoc begin */
    iterator_type begin() noexcept
    {
        const typename traits_type::difference_type pos = underlying_type::size();
        return iterator_type{&m_instances, pos};
    }

    /**
     * @brief Returns an iterator to the end.
     *
     * @return An iterator to the element following the last instance of the
     * given type.
     */
    const_iterator_type cend() const noexcept
    {
        return const_iterator_type{&m_instances, {}};
    }

    /*! @copydoc cend */
    const_iterator_type end() const noexcept
    {
        return cend();
    }

    /*! @copydoc end */
    iterator_type end() noexcept
    {
        return iterator_type{&m_instances, {}};
    }

    /**
     * @brief Returns the object associated with an entity.
     *
     * @param entt A valid entity identifier.
     * @return The object associated with the entity.
     */
    const object_type &get(const entity_type entt) const noexcept
    {
        return m_instances[underlying_type::index(entt)];
    }

    /*! @copydoc get */
    object_type &get(const entity_type entt) noexcept
    {
        return const_cast<object_type &>(std::as_const(*this).get(entt));
    }

    /**
     * @brief Returns a pointer to the object associated with an entity, if any.
     * @param entt A valid entity identifier.
     * @return The object associated with the entity, if any.
     */
    const object_type *try_get(const entity_type entt) const noexcept
    {
        return underlying_type::has(entt) ? (m_instances.data() + underlying_type::index(entt)) : nullptr;
    }

    /*! @copydoc try_get */
    object_type *try_get(const entity_type entt) noexcept
    {
        return const_cast<object_type *>(std::as_const(*this).try_get(entt));
    }

    /**
     * @brief Assigns an entity to a storage and constructs its object.
     *
     * @tparam Args Types of arguments to use to construct the object.
     * @param entt A valid entity identifier.
     * @param args Parameters to use to construct an object for the entity.
     * @return The object associated with the entity.
     */
    template <typename... Args>
    object_type &construct(const entity_type entt, Args &&... args)
    {
        if constexpr (std::is_aggregate_v<object_type>)
        {
            m_instances.emplace_back(Type{std::forward<Args>(args)...});
        }
        else
        {
            m_instances.emplace_back(std::forward<Args>(args)...);
        }

        // entity goes after component in case constructor throws
        underlying_type::construct(entt);
        return m_instances.back();
    }

    /**
     * @brief Assigns one or more entities to a storage and default constructs
     * or copy constructs their objects.
     *
     * @tparam It Type of forward iterator.
     * @tparam Args Types of arguments to use to construct the object.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param args Parameters to use to construct an object for the entities.
     * @return An iterator to the list of instances just created and sorted the
     * same of the entities.
     */
    template <typename It, typename... Args>
    iterator_type batch(It first, It last, Args &&... args)
    {
        if constexpr (sizeof...(Args) == 0)
        {
            m_instances.resize(m_instances.size() + std::distance(first, last));
        }
        else
        {
            m_instances.resize(m_instances.size() + std::distance(first, last), Type{std::forward<Args>(args)...});
        }

        // entity goes after component in case constructor throws
        underlying_type::batch(first, last);
        return begin();
    }

    /**
     * @brief Removes an entity from a storage and destroys its object.
     *
     * @param entt A valid entity identifier.
     */
    void destroy(const entity_type entt)
    {
        auto other = std::move(m_instances.back());
        m_instances[underlying_type::index(entt)] = std::move(other);
        m_instances.pop_back();
        underlying_type::destroy(entt);
    }

    /**
     * @brief Swaps entities and objects in the internal packed arrays.
     *
     * @param lhs A valid entity identifier.
     * @param rhs A valid entity identifier.
     */
    void swap(const entity_type lhs, const entity_type rhs) noexcept override
    {
        std::swap(m_instances[underlying_type::index(lhs)], m_instances[underlying_type::index(rhs)]);
        underlying_type::swap(lhs, rhs);
    }

    /*! @brief Resets a storage. */
    void reset()
    {
        underlying_type::reset();
        m_instances.clear();
    }

private:
    std::vector<object_type> m_instances;
};

/*! @copydoc basic_storage */
template <typename Entity, typename Type>
struct storage : basic_storage<Entity, Type>
{
};

} // namespace ssecs::ecs

#endif // STORAGE_HPP
