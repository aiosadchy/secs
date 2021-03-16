#ifndef SECS_COMPONENT_POOL_HPP
#define SECS_COMPONENT_POOL_HPP

#include "SECS/collections/sparse_map.hpp"
#include "SECS/entity.hpp"

class AbstractComponentPool {
public:
    virtual ~AbstractComponentPool() = default;
    virtual Index size() const = 0;
    virtual void remove(Entity::ID key) = 0;
    virtual bool contains(Entity::ID key) const = 0;

};

template <typename T>
class ComponentPool : public AbstractComponentPool {
private:
    template <typename Iterator>
    class GenericIterator {
    public:
        GenericIterator();
        explicit GenericIterator(const Iterator &iterator);

        GenericIterator &operator++();
        Entity::ID operator*();

        template <typename Another>
        bool operator!=(const Another &another) const;

    private:
        template <typename I>
        friend class GenericIterator;

        Iterator m_iterator;

    };

public:
    using Iterator      = GenericIterator<decltype(std::declval<SparseMap<T>>().keys().begin())>;
    using ConstIterator = GenericIterator<decltype(std::declval<const SparseMap<T>>().keys().begin())>;

    explicit ComponentPool(Index initial_capacity);
    ~ComponentPool() override = default;

    inline Iterator begin();
    inline Iterator end();

    inline ConstIterator begin() const;
    inline ConstIterator end() const;

    inline Index size() const override;

    // TODO: change input parameter type to Index because
    //       this class can not check if Entity::ID is valid

    template <typename ...Args>
    inline T &put(Entity::ID key, Args&& ...args);

    inline void remove(Entity::ID key) override;
    inline bool contains(Entity::ID key) const override;

    inline T &get(Entity::ID key);
    inline const T &get(Entity::ID key) const;

    inline T *find(Entity::ID key);
    inline const T *find(Entity::ID key) const;

private:
    SparseMap<T> m_data;

};

#endif // SECS_COMPONENT_POOL_HPP
