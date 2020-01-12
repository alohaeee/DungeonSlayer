#ifndef SPARCESET_HPP
#define SPARCESET_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#include "config.h"
#include "entity.hpp"
#include "fwd.hpp"

namespace ssecs::ecs
{

template <typename Entity,
          typename = std::enable_if_t<std::is_enum_v<Entity> && std::is_unsigned_v<std::underlying_type_t<Entity>>>>
class sparse_set
{
    using traits_type = entity_traits<std::underlying_type_t<Entity>>;

    static_assert(SSECS_PAGE_SIZE && ((SSECS_PAGE_SIZE & (SSECS_PAGE_SIZE - 1)) == 0));
    static constexpr auto entity_per_page = SSECS_PAGE_SIZE / sizeof(typename traits_type::entity_type);

    class iterator
    {
        friend class sparse_set<Entity>;

        using direct_type = const std::vector<Entity>;
        using index_type = typename traits_type::difference_type;

        iterator(direct_type *ref, const index_type idx) noexcept : m_direct{ref}, m_index{idx}
        {
        }

    public:
        using difference_type = index_type;
        using value_type = Entity;
        using pointer = const value_type *;
        using reference = const value_type &;
        using iterator_category = std::random_access_iterator_tag;

        iterator() noexcept = default;

        iterator &operator++() noexcept
        {
            return --m_index, *this;
        }

        iterator operator++(int) noexcept
        {
            iterator orig = *this;
            return ++(*this), orig;
        }

        iterator &operator--() noexcept
        {
            return ++m_index, *this;
        }

        iterator operator--(int) noexcept
        {
            iterator orig = *this;
            return --(*this), orig;
        }

        iterator &operator+=(const difference_type value) noexcept
        {
            m_index -= value;
            return *this;
        }

        iterator operator+(const difference_type value) const noexcept
        {
            return iterator{m_direct, m_index - value};
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
            return other.index - m_index;
        }

        reference operator[](const difference_type value) const noexcept
        {
            const auto pos = size_type(m_index - value - 1);
            return (*m_direct)[pos];
        }

        bool operator==(const iterator &other) const noexcept
        {
            return other.m_index == m_index;
        }

        bool operator!=(const iterator &other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const iterator &other) const noexcept
        {
            return m_index > other.m_index;
        }

        bool operator>(const iterator &other) const noexcept
        {
            return m_index < other.m_index;
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
            const auto pos = size_type(m_index - 1);
            return &(*m_direct)[pos];
        }

        reference operator*() const noexcept
        {
            return *operator->();
        }

    private:
        direct_type *m_direct;
        index_type m_index;
    };

    // Check whether is a need new page
    void assure(const std::size_t page)
    {
        // Add new page.
        if (!(page < m_sparce.size()))
        {
            m_sparce.resize(page + 1);
        }
        // Fill new page with null entities.
        if (!m_sparce[page])
        {
            m_sparce[page] = std::make_unique<entity_type[]>(entity_per_page);
            // null is safe in all cases for our purposes
            std::fill_n(m_sparce[page].get(), entity_per_page, null);
        }
    }
    // Maping Entity
    auto map(const Entity entt) const noexcept
    {
        const auto identifier = to_integer(entt) & traits_type::entity_mask;
        const auto page = size_type(identifier / entity_per_page);
        const auto offset = size_type(identifier & (entity_per_page - 1));
        return std::make_pair(page, offset);
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Random access iterator type. */
    using iterator_type = iterator;

    /*! @brief Default constructor. */
    sparse_set() = default;

    /**
     * @brief Copy constructor.
     * @param other The instance to copy from.
     */
    sparse_set(const sparse_set &other) : m_sparce{}, m_dence{other.m_dence}
    {
        for (size_type pos{}, last = other.m_sparce.size(); pos < last; ++pos)
        {
            if (other.m_sparce[pos])
            {
                assure(pos);
                std::copy_n(other.m_sparce[pos].get(), entity_per_page, m_sparce[pos].get());
            }
        }
    }

    /*! @brief Default move constructor. */
    sparse_set(sparse_set &&) = default;

    /*! @brief Default destructor. */
    virtual ~sparse_set() noexcept = default;

    /**
     * @brief Copy assignment operator.
     * @param other The instance to copy from.
     * @return This sparse set.
     */
    sparse_set &operator=(const sparse_set &other)
    {
        if (&other != this)
        {
            auto tmp{other};
            *this = std::move(tmp);
        }

        return *this;
    }

    /*! @brief Default move assignment operator. @return This sparse set. */
    sparse_set &operator=(sparse_set &&) = default;

    /**
     * @brief Increases the capacity of a sparse set.
     *
     * If the new capacity is greater than the current capacity, new storage is
     * allocated, otherwise the method does nothing.
     *
     * @param cap Desired capacity.
     */
    void reserve(const size_type cap)
    {
        m_dence.reserve(cap);
    }

    /**
     * @brief Returns the number of elements that a sparse set has currently
     * allocated space for.
     * @return Capacity of the sparse set.
     */
    size_type capacity() const noexcept
    {
        return m_dence.capacity();
    }

    /*! @brief Requests the removal of unused capacity. */
    void shrink_to_fit()
    {
        // conservative approach
        if (m_dence.empty())
        {
            m_sparce.clear();
        }

        m_sparce.shrink_to_fit();
        m_dence.shrink_to_fit();
    }

    /**
     * @brief Returns the extent of a sparse set(internal sparce array).
     * @return Extent of the sparse set.
     */
    size_type extent() const noexcept
    {
        return m_sparce.size() * entity_per_page;
    }

    /**
     * @brief Returns the number of elements in a sparse set(internal dence array).
     * @return Number of elements.
     */
    size_type size() const noexcept
    {
        return m_dence.size();
    }

    /**
     * @brief Checks whether a sparse set is empty.
     * @return True if the sparse set is empty, false otherwise.
     */
    bool empty() const noexcept
    {
        return m_dence.empty();
    }

    /**
     * @brief Direct access to the internal dence array.
     * @return A pointer to the internal dence array.
     */
    const entity_type *data() const noexcept
    {
        return m_dence.data();
    }

    /**
     * @brief Returns an iterator to the beginning.
     * @return An iterator to the first entity of the internal dence array.
     */
    iterator_type begin() const noexcept
    {
        const typename traits_type::difference_type pos = m_dence.size();
        return iterator_type{&m_dence, pos};
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last entity of the
     * internal dence array.
     */
    iterator_type end() const noexcept
    {
        return iterator_type{&m_dence, {}};
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid entity identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    iterator_type find(const entity_type entt) const noexcept
    {
        return has(entt) ? --(end() - index(entt)) : end();
    }

    /**
     * @brief Checks if a sparse set contains an entity.
     * @param entt A valid entity identifier.
     * @return True if the sparse set contains the entity, false otherwise.
     */
    bool has(const entity_type entity) const noexcept
    {
        auto [page, offset] = map(entity);
        return (page < m_sparce.size() && m_sparce[page] && m_sparce[page][offset] != null);
    }

    /**
     * @brief Returns the position of an entity in a sparse set.
     *
     * @warning
     * Attempting to get the position of an entity that doesn't belong to the
     * sparse set results in undefined behavior.<br/>
     * An SSECS_ASSERTion will abort the execution at runtime in debug mode if the
     * sparse set doesn't contain the given entity.
     *
     * @param entt A valid entity identifier.
     * @return The position of the entity in the sparse set.
     */
    size_type index(const entity_type entity) const noexcept
    {
        SSECS_ASSERT(has(entity));
        auto [page, offset] = map(entity);
        return size_type(m_sparce[page][offset]);
    }

    /**
     * @brief Assigns an entity to a sparse set.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the sparse set
     * results in undefined behavior.<br/>
     * An assertion will abort the execution at runtime in debug mode if the
     * sparse set already contains the given entity.
     *
     * @param entity A valid entity identifier.
     */
    void construct(const entity_type entity)
    {
        SSECS_ASSERT(!has(entity));
        auto [page, offset] = map(entity);
        assure(page);
        m_sparce[page][offset] = entity_type(m_dence.size());
        m_dence.push_back(entity);
    }

    /**
     * @brief Assigns one or more entities to a sparse set.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the sparse set
     * results in undefined behavior.<br/>
     * An assertion will abort the execution at runtime in debug mode if the
     * sparse set already contains the given entity.
     *
     * @tparam It Type of forward iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template <typename It>
    void batch(It first, It last)
    {
        std::for_each(first, last, [this, next = m_dence.size()](const auto entity) mutable {
            SSECS_ASSERT(!has(entity));
            auto [page, offset] = map(entity);
            assure(page);
            m_sparce[page][offset] = entity_type(next++);
        });

        m_dence.insert(m_dence.end(), first, last);
    }

    /**
     * @brief Removes an entity from a sparse set.
     *
     * @warning
     * Attempting to remove an entity that doesn't belong to the sparse set
     * results in undefined behavior.<br/>
     * An assertion will abort the execution at runtime in debug mode if the
     * sparse set doesn't contain the given entity.
     *
     * @param entt A valid entity identifier.
     */
    void destroy(const entity_type entt)
    {
        SSECS_ASSERT(has(entt));
        auto [from_page, from_offset] = map(entt);
        auto [to_page, to_offset] = map(m_dence.back());
        // swap with last element of dence arrray.
        m_dence[size_type(m_sparce[from_page][from_offset])] = entity_type(m_dence.back());
        m_sparce[to_page][to_offset] = m_sparce[from_page][from_offset];
        m_sparce[from_page][from_offset] = null;
        m_dence.pop_back();
    }

    /**
     * @brief Swaps two entities in the internal dence array.
     *
     * For what it's worth, this function affects both the internal sparse array
     * and the internal packed array.
     *
     * @warning
     * Attempting to swap entities that don't belong to the sparse set results
     * in undefined behavior.<br/>
     * An assertion will abort the execution at runtime in debug mode if the
     * sparse set doesn't contain the given entities.
     *
     * @param lhs A valid entity identifier.
     * @param rhs A valid entity identifier.
     */
    virtual void swap(const entity_type lhs, const entity_type rhs) noexcept
    {
        auto [src_page, src_offset] = map(lhs);
        auto [dst_page, dst_offset] = map(rhs);
        auto &from = m_sparce[src_page][src_offset];
        auto &to = m_sparce[dst_page][dst_offset];
        std::swap(m_dence[size_type(from)], m_dence[size_type(to)]);
        std::swap(from, to);
    }

    /**
     * @brief Sort entities according to their order in another sparse set.
     *
     * Entities that are part of both the sparse sets are ordered internally
     * according to the order they have in `other`. All the other entities goes
     * to the end of the list and there are no guarantees on their order.<br/>
     * In other terms, this function can be used to impose the same order on two
     * sets by using one of them as a master and the other one as a slave.
     *
     * Iterating the sparse set with a couple of iterators returns elements in
     * the expected order after a call to `respect`. See `begin` and `end` for
     * more details.
     *
     * @note
     * Attempting to iterate elements using a raw pointer returned by a call to
     * `data` gives no guarantees on the order, even though `respect` has been
     * invoked.
     *
     * @param other The sparse sets that imposes the order of the entities.
     */
    void respect(const sparse_set &other) noexcept
    {
        const auto to = other.end();
        auto from = other.begin();

        size_type pos = m_dence.size() - 1;

        while (pos && from != to)
        {
            if (has(*from))
            {
                if (*from != m_dence[pos])
                {
                    swap(m_dence[pos], *from);
                }

                --pos;
            }

            ++from;
        }
    }

    /**
     * @brief Resets a sparse set.
     */
    void reset()
    {
        m_sparce.clear();
        m_dence.clear();
    }

private:
    std::vector<std::unique_ptr<entity_type[]>> m_sparce;
    std::vector<entity_type> m_dence;
};
} // namespace ssecs::ecs

#endif // SPARCESET_HPP
