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

protected:
    class EntityToIndex {
    public:
        Index operator()(const Entity::ID &entity) const noexcept;

    };

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
        bool operator!=(const GenericIterator &another) const;

    private:
        Iterator m_iterator;

    };

    using Map = SparseMap<Entity::ID, T, EntityToIndex>;

public:
    using Iterator      = GenericIterator<decltype(std::declval<Map>().keys().begin())>;
    using ConstIterator = GenericIterator<decltype(std::declval<const Map>().keys().begin())>;

    explicit ComponentPool(Index initial_capacity);
    ~ComponentPool() override = default;

    inline Iterator begin();
    inline Iterator end();

    inline ConstIterator begin() const;
    inline ConstIterator end() const;

    inline Index size() const override;

    template <typename ...Args>
    inline T &put(Entity::ID key, Args&& ...args);

    inline void remove(Entity::ID key) override;
    inline bool contains(Entity::ID key) const override;

    inline T &get(Entity::ID key);
    inline const T &get(Entity::ID key) const;

    inline T *find(Entity::ID key);
    inline const T *find(Entity::ID key) const;

private:
    Map m_data;

};

#endif // SECS_COMPONENT_POOL_HPP
