#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "entity.hpp"
#include "family.hpp"
#include "fwd.hpp"
#include "storage.hpp"
#include "type_traits.hpp"
#include "view.hpp"

namespace ssecs::ecs
{

/*! @brief Container and manager for pools and entities. */
template <typename Entity>
class basic_registry
{
    using traits_type = entity_traits<std::underlying_type_t<Entity>>;
    using component_family = family<struct internal_registry_component_family>;

    template <typename Component>
    struct pool_handler : storage<Entity, Component>
    {
        template <typename... cArgs>
        decltype(auto) assign([[maybe_unused]] basic_registry &owner, const Entity entity, cArgs &&... args)
        {
            return storage<Entity, Component>::construct(entity, std::forward<cArgs>(args)...);
        }

        template <typename It, typename... Args>
        auto batch([[maybe_unused]] basic_registry &owner, It first, It last, Args &&... args)
        {
            auto it = storage<Entity, Component>::batch(first, last, std::forward<Args>(args)...);
            return it;
        }

        void remove([[maybe_unused]] basic_registry &owner, const Entity entity)
        {
            storage<Entity, Component>::remove(entity);
        }

        template <typename... cArgs>
        decltype(auto) replace([[maybe_unused]] basic_registry &owner, const Entity entity, cArgs &&... args)
        {
            Component component{std::forward<cArgs>(args)...};
            return (storage<Entity, Component>::get(entity) = std::move(component));
        }
    };
    template <typename Component>
    using pool_type = pool_handler<std::decay_t<Component>>;

    struct pool_data
    {
        std::unique_ptr<sparse_set<Entity>> pool;

        SSECS_ID_TYPE runtime_type;
    };

    template <typename Type, typename Family>
    static SSECS_ID_TYPE runtime_type() noexcept
    {
        return Family::template type<std::decay_t<Type>>;
    }

    template <typename Component>
    const pool_type<Component> *assure() const
    {
        const auto ctype = to_integer(type<Component>());
        pool_data *pdata = nullptr;

        // if ctype is new family
        if (!(ctype < m_skip_family_pools))
        {
            m_pools.reserve(m_pools.size() + ctype - m_skip_family_pools + 1);

            while (!(ctype < m_skip_family_pools))
            {
                m_pools.emplace(m_pools.begin() + (m_skip_family_pools++), pool_data{});
            }
        }
        pdata = &m_pools[ctype];
        if (!pdata->pool)
        {

            pdata->runtime_type = ctype;
            pdata->pool = std::make_unique<pool_type<Component>>();
        }

        return static_cast<pool_type<Component> *>(pdata->pool.get());
    }
    template <typename Component>
    pool_type<Component> *assure()
    {
        return const_cast<pool_type<Component> *>(std::as_const(*this).template assure<Component>());
    }
    auto generate()
    {
        Entity entity;

        if (m_destroyed == null)
        {
            entity = m_entities.emplace_back(entity_type{m_entities.size()});
            SSECS_ASSERT(to_integer(entity) < traits_type::entity_mask);
        }
        else
        {
            const auto curr = to_integer(m_destroyed);
            const auto version = to_integer(m_entities[static_cast<size_type>(curr)]) &
                                 (traits_type::version_mask << traits_type::entity_shift);
            // erase fron current entity from opaque list
            m_destroyed = entity_type{to_integer(m_entities[static_cast<size_type>(curr)]) & traits_type::entity_mask};
            entity = entity_type{curr | version};
            m_entities[curr] = entity;
        }
        return entity;
    }
    void release(const Entity entity)
    {
        // lengthens the implicit list of destroyed entities
        const auto id = to_integer(entity) & traits_type::entity_mask;
        const auto version = ((to_integer(entity) >> traits_type::entity_shift) + 1) << traits_type::entity_shift;
        const auto node = to_integer(m_destroyed) | version;
        m_entities[id] = Entity{node};
        m_destroyed = Entity{id};
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Underlying version type. */
    using version_type = typename traits_type::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;

    /*! @brief Default constructor. */
    basic_registry() noexcept = default;
    /*! @brief Default move constructor. */
    basic_registry(basic_registry &&) = default;
    /*! @brief Default move assignment operator. @return This basic_registry. */
    basic_registry &operator=(basic_registry &&) = default;
    /*! @brief Default destructor */
    ~basic_registry() = default;

    basic_registry(std::size_t allocate_size)
    {
        m_destroyed.resize(allocate_size);

        for (std::size_t i = 0; i < allocate_size; m_destroyed[i] = i++)
            ;
    }

    /**
     * @brief Return runtime type.
     * @return Runtime type id.
     */
    template <typename Component>
    static auto type() noexcept
    {
        return component{runtime_type<Component, component_family>()};
    }

    /**
     * @brief Returns the entity identifier without the version.
     * @param entity An entity identifier, either valid or not.
     * @return The entity identifier without the version.
     */
    static entity_type entity(const entity_type entity) noexcept
    {
        return entity_type{to_integer(entity) & traits_type::entity_mask};
    }

    /**
     * @brief Returns the version stored along with an entity identifier.
     * @param entity An entity identifier, either valid or not.
     * @return The version stored along with the given entity identifier.
     */
    static version_type version(const entity_type entity) noexcept
    {
        return version_type(to_integer(entity) >> traits_type::entity_shift);
    }

    /**
     * @brief Checks if an entity identifier refers to a valid entity.
     * @param entity An entity identifier, either valid or not.
     * @return True if the identifier is valid, false otherwise.
     */
    bool valid(const entity_type entity) const noexcept
    {
        const auto pos = size_type(to_integer(entity) & traits_type::entity_mask);
        return (pos < m_entities.size() && m_entities[pos] == entity);
    }

    /**
     * @brief Returns the actual version for an entity identifier.
     * @param entity A valid entity identifier.
     * @return Actual version for the given entity identifier.
     */
    version_type current(const entity_type entity) const noexcept
    {
        const auto pos = size_type(to_integer(entity) & traits_type::entity_mask);
        SSECS_ASSERT(pos < m_entities.size());
        return version_type(to_integer(m_entities[pos]) >> traits_type::entity_shift);
    }

    /**
     * @brief Returns the number of existing components of the given type.
     * @tparam Component Type of component of which to return the size.
     * @return Number of existing components of the given type.
     */
    template <typename Component>
    size_type size() const noexcept
    {
        return assure<Component>()->size();
    }

    /**
     * @brief Returns the number of entities created so far.
     * @return Number of entities created so far.
     */
    size_type size() const noexcept
    {
        return m_entities.size();
    }

    /**
     * @brief Returns the number of entities still in use.
     * @return Number of entities still in use.
     */
    size_type alive() const noexcept
    {
        auto count = m_entities.size();
        auto current = m_destroyed;

        for (; current != null; --count)
        {
            current = m_entities[to_integer(current) & traits_type::entity_mask];
        }

        return count;
    }

    /**
     * @brief Increases the capacity of the registry or of the pools for the
     * given components.
     *
     * If no components are specified, the capacity of the registry is
     * increased, that is the number of entities it contains. Otherwise the
     * capacity of the pools for the given components is increased.<br/>
     * In both cases, if the new capacity is greater than the current capacity,
     * new storage is allocated, otherwise the method does nothing.
     *
     * @tparam Component Types of components for which to reserve storage.
     * @param capacity Desired capacity.
     */
    template <typename... Component>
    void reserve(const size_type capacity)
    {
        if constexpr (sizeof...(Component) == 0)
        {
            m_entities.reserve(capacity);
        }
        else
        {
            (assure<Component>()->reserve(capacity), ...);
        }
    }

    /**
     * @brief Returns the capacity of the pool for the given component.
     * @tparam Component Type of component in which one is interested.
     * @return Capacity of the pool of the given component.
     */
    template <typename Component>
    size_type capacity() const noexcept
    {
        return assure<Component>()->capacity();
    }

    /**
     * @brief Returns the number of entities that a registry has currently
     * allocated space for.
     * @return Capacity of the registry.
     */
    size_type capacity() const noexcept
    {
        return m_entities.capacity();
    }

    /**
     * @brief Requests the removal of unused capacity for the given components.
     * @tparam Component Types of components for which to reclaim unused
     * capacity.
     */
    template <typename... Component>
    void shrink_to_fit()
    {
        (assure<Component>()->shrink_to_fit(), ...);
    }

    /**
     * @brief Checks whether the registry or the pools of the given components
     * are empty.
     *
     * A registry is considered empty when it doesn't contain entities that are
     * still in use.
     *
     * @tparam Component Types of components in which one is interested.
     * @return True if the registry or the pools of the given components are
     * empty, false otherwise.
     */
    template <typename... Component>
    bool empty() const noexcept
    {
        if constexpr (sizeof...(Component) == 0)
        {
            return !alive();
        }
        else
        {
            return (assure<Component>()->empty() && ...);
        }
    }

    /**
     * @brief Direct acsess to the internal list of components.
     * @return Pointer to the internal array of components
     */
    template <typename Component>
    const Component *raw() const noexcept
    {
        return assure<Component>()->raw();
    }

    /**
     * @brief Direct acsess to the internal list of the entitites that
     * obtains typed Component.
     * @return Poiner to the array of entities.
     */
    template <typename Component>
    const entity_type *data() const noexcept
    {
        return assure<Component>()->data();
    }

    /**
     * @brief Create a new entity and returns it.
     *
     * @note There are two kinds of entity identifiers:
     * * Newly created ones in case no entities have been previously destroyed.
     * * Recycled ones with updated versions.
     *
     * @tparam Component Types of components to assign to the entity.
     * @return Created entity.
     */
    template <typename... Component>
    auto create()
    {
        if constexpr (sizeof...(Component) == 0)
        {
            return generate();
        }
        else
        {
            const entity_type entt = generate();
            return std::tuple<entity_type, decltype(assign<Component>({}))...>{entt, assign<Component>(entt)...};
        }
    }

    /**
     * @brief Destroys an entity and lets the registry recycle the identifier.
     * @param entity A valid entity identifier.
     */
    void destroy(const entity_type entity)
    {
        SSECS_ASSERT(valid(entity));

        for (auto pos = m_pools.size(); pos; --pos)
        {
            if (auto &pdata = m_pools[pos - 1]; pdata.pool && pdata.pool->has(entity))
            {
                pdata.remove(*pdata.pool, *this, entity);
            }
        }

        release(entity);
    }

    /**
     * @brief Assign component to the entity.
     *
     * @tparam Component Type of component to assign to the entity.
     * @tparam cArgs Types of component's construct arguments.
     * @param entity A valid entity identifier.
     * @param args Parameters to initialize component.
     *
     * @return A reference to the newly created component.
     */
    template <typename Component, typename... cArgs>
    decltype(auto) assign(const entity_type entity, cArgs &&... args)
    {
        SSECS_ASSERT(valid(entity));
        return assure<Component>()->assign(*this, entity, std::forward<cArgs>(args)...);
    }

    /**
     * @brief Remove component from the entity.
     * @param entity A valid entity identifier.
     */
    template <typename Component>
    void remove(const entity_type entity) noexcept
    {
        SSECS_ASSERT(valid(entity));
        assure<Component>()->destroy(entity);
    }

    /**
     * @brief Checks if an entity has all the given components.
     *
     * @warning
     * Attempting to use an invalid entity results in undefined behavior.<br/>
     * An SSECS_ASSERTion will abort the execution at runtime in debug mode in case of
     * invalid entity.
     *
     * @tparam Component Components for which to perform the check.
     * @param entity A valid entity identifier.
     * @return True if the entity has all the components, false otherwise.
     */
    template <typename... Component>
    bool has(const entity_type entity) const noexcept
    {
        SSECS_ASSERT(valid(entity));
        return (assure<Component>()->has(entity) && ...);
    }

    /**
     * @brief Get Components assigned to the entity.
     *
     * @tparam Component Desired components.
     * @param entity A valid entity identifier.
     *
     * @return Tuple of assigned components.
     */
    template <typename... Component>
    decltype(auto) get(const entity_type entity) const noexcept
    {
        SSECS_ASSERT(valid(entity));

        if constexpr (sizeof...(Component) == 1)
        {
            return (assure<Component>()->get(entity), ...);
        }
        else
        {
            return std::tuple<decltype(get<Component>({}))...>{get<Component>(entity)...};
        }
    }
    /*! @copydoc get */
    template <typename... Component>
    decltype(auto) get([[maybe_unused]] const entity_type entity) noexcept
    {
        SSECS_ASSERT(valid(entity));

        if constexpr (sizeof...(Component) == 1)
        {
            return (assure<Component>()->get(entity), ...);
        }
        else
        {
            return std::tuple<decltype(get<Component>({}))...>{get<Component>(entity)...};
        }
    }

    /**
     *  @brief Replaces the given component for an entity.
     *
     *  @tparam Component Type of component to replace.
     *  @tparam Args Types of arguments to use to construct the component.
     *  @param entity A valid entity identifier.
     *  @param args Parameters to use to initialize the component.
     *  @return A reference to the newly created component.
     */
    template <typename Component, typename... cArgs>
    decltype(auto) replace(const entity_type entity, cArgs &&... args)
    {
        SSECS_ASSERT(valid(entity));

        return assure<Component>()->replace(*this, entity, std::forward<cArgs>(args)...);
    }

    /**
     * @brief Resets the given component for an entity.
     *
     * If the entity has an instance of the component, this function removes the
     * component from the entity. Otherwise it does nothing.
     *
     * @tparam Component Type of component to reset.
     * @param entity A valid entity identifier.
     */
    template <typename Component>
    void reset(const entity_type entity)
    {
        ENTT_SSECS_ASSERT(valid(entity));

        if (auto *cpool = assure<Component>(); cpool->has(entity))
        {
            cpool->remove(*this, entity);
        }
    }
    /**
     * @brief Resets the pool of the given component.
     *
     * For each entity that has an instance of the given component, the
     * component itself is removed and thus destroyed.
     *
     * @tparam Component Type of component whose pool must be reset.
     */
    template <typename Component>
    void reset()
    {
        if (auto *cpool = assure<Component>(); cpool->on_destroy().empty())
        {
            // no group set, otherwise the signal wouldn't be empty
            cpool->reset();
        }
        else
        {
            for (const auto entity : static_cast<const sparse_set<entity_type> &>(*cpool))
            {
                cpool->remove(*this, entity);
            }
        }
    }

    /**
     * @brief Iterates all the entities that are still in use.
     *
     * The function object is invoked for each entity that is still in use.<br/>
     * The signature of the function should be equivalent to the following:
     *
     * @code{.cpp}
     * void(const Entity);
     * @endcode
     *
     * This function is fairly slow and should not be used frequently. However,
     * it's useful for iterating all the entities still in use, regardless of
     * their components.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template <typename Func>
    void each(Func func) const
    {
        static_assert(std::is_invocable_v<Func, entity_type>);

        if (m_destroyed == null)
        {
            for (auto pos = m_entities.size(); pos; --pos)
            {
                func(m_entities[pos - 1]);
            }
        }
        else
        {
            for (auto pos = m_entities.size(); pos; --pos)
            {
                const auto curr = entity_type(pos - 1);
                const auto entity = m_entities[to_integer(curr)];
                const auto entt = entity_type{to_integer(entity) & traits_type::entity_mask};

                if (curr == entt)
                {
                    func(entity);
                }
            }
        }
    }

    /**
     * @brief Resets a whole registry.
     *
     * Destroys all the entities. After a call to `reset`, all the entities
     * still in use are recycled with a new version number. In case entity
     * identifers are stored around, the `valid` member function can be used
     * to know if they are still valid.
     */
    void reset()
    {
        each([this](const auto entity) {
            // useless this-> used to suppress a warning with clang
            this->destroy(entity);
        });
    }
    /**
     * @brief Returns a view for the given components.
     *
     * This kind of objects are created on the fly and share with the registry
     * its internal data structures.<br/>
     * Feel free to discard a view after the use. Creating and destroying a view
     * is an incredibly cheap operation because they do not require any type of
     * initialization.<br/>
     * As a rule of thumb, storing a view should never be an option.
     *
     * Views do their best to iterate the smallest set of candidate entities.
     * In particular:
     *
     * * Single component views are incredibly fast and iterate a packed array
     *   of entities, all of which has the given component.
     * * Multi component views look at the number of entities available for each
     *   component and pick up a reference to the smallest set of candidates to
     *   test for the given components.
     *
     * Views in no way affect the functionalities of the registry nor those of
     * the underlying pools.
     *
     *
     * @tparam Component Type of components used to construct the view.
     * @tparam Exclude Types of components used to filter the view.
     * @return A newly created view.
     */
    template <typename... Component, typename... Exclude>
    basic_view<Entity, exclude_t<Exclude...>, Component...> view(exclude_t<Exclude...> = {})
    {
        static_assert(sizeof...(Component) > 0);
        return {assure<Component>()..., assure<Exclude>()...};
    }

    /*! @copydoc view */
    template <typename... Component, typename... Exclude>
    basic_view<Entity, exclude_t<Exclude...>, Component...> view(exclude_t<Exclude...> = {}) const
    {
        static_assert(std::conjunction_v<std::is_const<Component>...>);
        return const_cast<basic_registry *>(this)->view<Component...>(exclude<Exclude...>);
    }

private:
    std::vector<entity_type> m_entities{};
    Entity m_destroyed{null};

    mutable std::vector<pool_data> m_pools{};
    mutable size_type m_skip_family_pools{};
};
} // namespace ssecs::ecs

#endif // REGISTRY_HPP
