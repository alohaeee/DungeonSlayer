#ifndef VIEW_HPP
#define VIEW_HPP

#include <algorithm>
#include <array>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include "config.h"
#include "entity.hpp"
#include "fwd.hpp"
#include "sparse_set.hpp"
#include "storage.hpp"
#include "type_traits.hpp"
#include "utility.hpp"

namespace ssecs::ecs
{

/**
 * @brief View.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error, but for a few reasonable cases.
 */
template <typename...>
class basic_view;

/**
 * @brief Multi component view.
 *
 * Multi component views iterate over those entities that have at least all the
 * given components in their bags. During initialization, a multi component view
 * looks at the number of entities available for each component and picks up a
 * reference to the smallest set of candidate entities in order to get a
 * performance boost when iterate.
 *
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Exclude Types of components used to filter the view.
 * @tparam Component Types of components iterated by the view.
 */
template <typename Entity, typename... Exclude, typename... Component>
class basic_view<Entity, exclude_t<Exclude...>, Component...>
{
    /*! @brief A registry is allowed to create views. */
    friend class basic_registry<Entity>;

    template <typename Comp>
    using pool_type = std::conditional_t<std::is_const_v<Comp>, const storage<Entity, std::remove_const_t<Comp>>,
                                         storage<Entity, Comp>>;

    template <typename Comp>
    using component_iterator_type = decltype(std::declval<pool_type<Comp>>().begin());

    using underlying_iterator_type = typename sparse_set<Entity>::iterator_type;
    using unchecked_type = std::array<const sparse_set<Entity> *, (sizeof...(Component) - 1)>;
    using filter_type = std::array<const sparse_set<Entity> *, sizeof...(Exclude)>;

    class iterator
    {
        friend class basic_view<Entity, exclude_t<Exclude...>, Component...>;

        iterator(underlying_iterator_type first, underlying_iterator_type last, unchecked_type other,
                 filter_type ignore) noexcept
            : m_begin{first}, m_end{last}, m_unchecked{other}, m_filter{ignore}
        {
            if (m_begin != m_end && !valid())
            {
                ++(*this);
            }
        }

        // check whether another components have same entity.
        bool valid() const noexcept
        {

            return std::all_of(m_unchecked.cbegin(), m_unchecked.cend(),
                               [this](const sparse_set<Entity> *view) { return view->has(*m_begin); }) &&
                   std::none_of(m_filter.cbegin(), m_filter.cend(),
                                [this](const sparse_set<Entity> *view) { return view->has(*m_begin); });
        }

    public:
        using difference_type = typename underlying_iterator_type::difference_type;
        using value_type = typename underlying_iterator_type::value_type;
        using pointer = typename underlying_iterator_type::pointer;
        using reference = typename underlying_iterator_type::reference;
        using iterator_category = std::forward_iterator_tag;

        iterator() noexcept = default;

        iterator &operator++() noexcept
        {
            return (++m_begin != m_end && !valid()) ? ++(*this) : *this;
        }

        iterator operator++(int) noexcept
        {
            iterator orig = *this;
            return ++(*this), orig;
        }

        bool operator==(const iterator &other) const noexcept
        {
            return other.m_begin == m_begin;
        }

        bool operator!=(const iterator &other) const noexcept
        {
            return !(*this == other);
        }

        pointer operator->() const noexcept
        {
            return m_begin.operator->();
        }

        reference operator*() const noexcept
        {
            return *operator->();
        }

    private:
        underlying_iterator_type m_begin;
        underlying_iterator_type m_end;
        unchecked_type m_unchecked;
        filter_type m_filter;
    };

    // we could use pool_type<Component> *..., but vs complains about it and refuses to compile for unknown reasons
    // (likely a bug)
    basic_view(storage<Entity, std::remove_const_t<Component>> *... component,
               storage<Entity, std::remove_const_t<Exclude>> *... epool) noexcept
        : m_pools{component...}, m_filter{epool...}
    {
    }

    const sparse_set<Entity> *candidate() const noexcept
    {
        return std::min({static_cast<const sparse_set<Entity> *>(std::get<pool_type<Component> *>(m_pools))...},
                        [](const auto *lhs, const auto *rhs) { return lhs->size() < rhs->size(); });
    }

    // make unchecked array.
    unchecked_type unchecked(const sparse_set<Entity> *view) const noexcept
    {
        std::size_t pos{};
        unchecked_type other{};
        ((std::get<pool_type<Component> *>(m_pools) == view
              ? nullptr
              : (other[pos++] = std::get<pool_type<Component> *>(m_pools))),
         ...);
        return other;
    }

    template <typename Comp, typename Other>
    decltype(auto) get([[maybe_unused]] component_iterator_type<Comp> it, [[maybe_unused]] pool_type<Other> *cpool,
                       [[maybe_unused]] const Entity entt) const noexcept
    {
        if constexpr (std::is_same_v<Comp, Other>)
        {
            return *it;
        }
        else
        {
            return cpool->get(entt);
        }
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Input iterator type. */
    using iterator_type = iterator;

    /**
     * @brief Returns the number of existing components of the given type.
     *
     * This isn't the number of entities iterated by the view.
     *
     * @tparam Comp Type of component of which to return the size.
     * @return Number of existing components of the given type.
     */
    template <typename Comp>
    size_type size() const noexcept
    {
        return std::get<pool_type<Comp> *>(m_pools)->size();
    }

    /**
     * @brief Estimates the number of entities iterated by the view.
     * @return Estimated number of entities iterated by the view.
     */
    size_type size() const noexcept
    {
        return std::min({std::get<pool_type<Component> *>(m_pools)->size()...});
    }

    /**
     * @brief Checks whether the view or the pools of the given components are
     * empty.
     * @tparam Comp Types of components in which one is interested.
     * @return True if the view or the pools of the given components are empty,
     * false otherwise.
     */
    template <typename... Comp>
    bool empty() const noexcept
    {
        if constexpr (sizeof...(Comp) == 0)
        {
            return (std::get<pool_type<Component> *>(m_pools)->empty() || ...);
        }
        else
        {
            return (std::get<pool_type<Comp> *>(m_pools)->empty() && ...);
        }
    }

    /**
     * @brief Direct access to the list of components of a given pool.
     * @tparam Comp Type of component in which one is interested.
     * @return A pointer to the array of components.
     */
    template <typename Comp>
    Comp *raw() const noexcept
    {
        return std::get<pool_type<Comp> *>(m_pools)->raw();
    }

    /**
     * @brief Direct access to the list of entities of a given pool.
     * @tparam Comp Type of component in which one is interested.
     * @return A pointer to the array of entities.
     */
    template <typename Comp>
    const entity_type *data() const noexcept
    {
        return std::get<pool_type<Comp> *>(m_pools)->data();
    }

    /**
     * @brief Returns an iterator to the first entity that has the given
     * components.
     * @return An iterator to the first entity that has the given components.
     */
    iterator_type begin() const noexcept
    {
        const auto *view = candidate();
        const filter_type ignore{std::get<pool_type<Exclude> *>(m_filter)...};
        return iterator_type{view->begin(), view->end(), unchecked(view), ignore};
    }

    /**
     * @brief Returns an iterator that is past the last entity that has the
     * given components.
     *
     * @return An iterator to the entity following the last entity that has the
     * given components.
     */
    iterator_type end() const noexcept
    {
        const auto *view = candidate();
        const filter_type ignore{std::get<pool_type<Exclude> *>(m_filter)...};
        return iterator_type{view->end(), view->end(), unchecked(view), ignore};
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid entity identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    iterator_type find(const entity_type entt) const noexcept
    {
        const auto *view = candidate();
        const filter_type ignore{std::get<pool_type<Exclude> *>(m_filter)...};
        iterator_type it{view->find(entt), view->end(), unchecked(view), ignore};
        return (it != end() && *it == entt) ? it : end();
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid entity identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    bool contains(const entity_type entt) const noexcept
    {
        return find(entt) != end();
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * Prefer this function instead of `registry::get` during iterations. It has
     * far better performance than its companion function.
     *
     * @tparam Comp Types of components to get.
     * @param entt A valid entity identifier.
     * @return The components assigned to the entity.
     */
    template <typename... Comp>
    decltype(auto) get([[maybe_unused]] const entity_type entt) const noexcept
    {
        assert(contains(entt));

        if constexpr (sizeof...(Comp) == 0)
        {
            static_assert(sizeof...(Component) == 1);
            return (std::get<pool_type<Component> *>(m_pools)->get(entt), ...);
        }
        else if constexpr (sizeof...(Comp) == 1)
        {
            return (std::get<pool_type<Comp> *>(m_pools)->get(entt), ...);
        }
        else
        {
            return std::tuple<decltype(get<Comp>({}))...>{get<Comp>(entt)...};
        }
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a set of references to all its components. The
     * _constness_ of the components is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Component &...);
     * void(Component &...);
     * @endcode
     *
     * @note
     * Empty types aren't explicitly instantiated. Therefore, temporary objects
     * are returned during iterations. They can be caught only by copy or with
     * const references.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template <typename Func>
    void each(Func func) const
    {
        const auto *view = candidate();
        ((std::get<pool_type<Component> *>(m_pools) == view ? each<Component>(std::move(func)) : void()), ...);
    }

private:
    const std::tuple<pool_type<Component> *...> m_pools;
    const std::tuple<pool_type<Exclude> *...> m_filter;
};

/**
 * @brief Single component view specialization.
 *
 * Single component views are specialized in order to get a boost in terms of
 * performance. This kind of views can access the underlying data structure
 * directly and avoid superfluous checks.
 *
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Component Type of component iterated by the view.
 */
template <typename Entity, typename Component>
class basic_view<Entity, exclude_t<>, Component>
{
    /*! @brief A registry is allowed to create views. */
    friend class basic_registry<Entity>;

    using pool_type =
        std::conditional_t<std::is_const_v<Component>, const storage<Entity, std::remove_const_t<Component>>,
                           storage<Entity, Component>>;

    basic_view(pool_type *ref) noexcept : pool{ref}
    {
    }

public:
    /*! @brief Type of component iterated by the view. */
    using raw_type = Component;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Input iterator type. */
    using iterator_type = typename sparse_set<Entity>::iterator_type;

    /**
     * @brief Returns the number of entities that have the given component.
     * @return Number of entities that have the given component.
     */
    size_type size() const noexcept
    {
        return pool->size();
    }

    /**
     * @brief Checks whether the view is empty.
     * @return True if the view is empty, false otherwise.
     */
    bool empty() const noexcept
    {
        return pool->empty();
    }

    /**
     * @brief Direct access to the list of components.
     *
     * The returned pointer is such that range `[raw(), raw() + size()]` is
     * always a valid range, even if the container is empty.
     *
     * @note
     * There are no guarantees on the order of the components. Use `begin` and
     * `end` if you want to iterate the view in the expected order.
     *
     * @return A pointer to the array of components.
     */
    raw_type *raw() const noexcept
    {
        return pool->raw();
    }

    /**
     * @brief Direct access to the list of entities.
     *
     * The returned pointer is such that range `[data(), data() + size()]` is
     * always a valid range, even if the container is empty.
     *
     * @note
     * There are no guarantees on the order of the entities. Use `begin` and
     * `end` if you want to iterate the view in the expected order.
     *
     * @return A pointer to the array of entities.
     */
    const entity_type *data() const noexcept
    {
        return pool->data();
    }

    /**
     * @brief Returns an iterator to the first entity that has the given
     * component.
     *
     * The returned iterator points to the first entity that has the given
     * component. If the view is empty, the returned iterator will be equal to
     * `end()`.
     *
     * @note
     * Input iterators stay true to the order imposed to the underlying data
     * structures.
     *
     * @return An iterator to the first entity that has the given component.
     */
    iterator_type begin() const noexcept
    {
        return pool->sparse_set<Entity>::begin();
    }

    /**
     * @brief Returns an iterator that is past the last entity that has the
     * given component.
     *
     * The returned iterator points to the entity following the last entity that
     * has the given component. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
     * @note
     * Input iterators stay true to the order imposed to the underlying data
     * structures.
     *
     * @return An iterator to the entity following the last entity that has the
     * given component.
     */
    iterator_type end() const noexcept
    {
        return pool->sparse_set<Entity>::end();
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid entity identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    iterator_type find(const entity_type entt) const noexcept
    {
        const auto it = pool->find(entt);
        return it != end() && *it == entt ? it : end();
    }

    /**
     * @brief Returns the identifier that occupies the given position.
     * @param pos Position of the element to return.
     * @return The identifier that occupies the given position.
     */
    entity_type operator[](const size_type pos) const noexcept
    {
        return begin()[pos];
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid entity identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    bool contains(const entity_type entt) const noexcept
    {
        return find(entt) != end();
    }

    /**
     * @brief Returns the component assigned to the given entity.
     *
     * Prefer this function instead of `registry::get` during iterations. It has
     * far better performance than its companion function.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.<br/>
     * An assertion will abort the execution at runtime in debug mode if the
     * view doesn't contain the given entity.
     *
     * @param entt A valid entity identifier.
     * @return The component assigned to the entity.
     */
    template <typename Comp = Component>
    decltype(auto) get(const entity_type entt) const noexcept
    {
        static_assert(std::is_same_v<Comp, Component>);
        assert(contains(entt));
        return pool->get(entt);
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a reference to its component. The _constness_ of the
     * component is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Component &);
     * void(Component &);
     * @endcode
     *
     * @note
     * Empty types aren't explicitly instantiated. Therefore, temporary objects
     * are returned during iterations. They can be caught only by copy or with
     * const references.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template <typename Func>
    void each(Func func) const
    {
        if constexpr (std::is_invocable_v<Func, decltype(get({}))>)
        {
            std::for_each(pool->begin(), pool->end(), std::move(func));
        }
        else
        {
            std::for_each(pool->sparse_set<Entity>::begin(), pool->sparse_set<Entity>::end(),
                          [&func, raw = pool->begin()](const auto entt) mutable { func(entt, *(raw++)); });
        }
    }

private:
    pool_type *pool;
};

} // namespace ssecs::ecs

#endif // VIEW_HPP
