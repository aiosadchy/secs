#ifndef SECS_COMPONENT_POOL_HPP
#define SECS_COMPONENT_POOL_HPP

#include "SECS/collections/sparse_map.hpp"
#include "SECS/entity.hpp"


namespace secs {

class IComponentPool {
public:
    virtual ~IComponentPool() = default;
    virtual void remove(Entity entity) = 0;

};

template <typename T>
class ComponentPool : public IComponentPool {
private:
    template <typename Iterator>
    class GenericIterator {
    public:
        GenericIterator();
        explicit GenericIterator(const Iterator &iterator);

        GenericIterator &operator++();
        Entity operator*();
        bool operator!=(const GenericIterator &another) const;

    private:
        Iterator m_iterator;

    };

    using Map = SparseMap<Entity, T, Index (*)(Entity)>;

public:
    using Iterator      = GenericIterator<decltype(std::declval<Map>().keys().begin())>;
    using ConstIterator = GenericIterator<decltype(std::declval<const Map>().keys().begin())>;
    using End      = GenericIterator<decltype(std::declval<Map>().keys().end())>;
    using ConstEnd = GenericIterator<decltype(std::declval<const Map>().keys().end())>;

    explicit ComponentPool(Index initial_capacity);
    ~ComponentPool() override = default;

    inline Iterator begin();
    inline End end();

    inline ConstIterator begin() const;
    inline ConstEnd end() const;

    inline Index size() const;

    template <typename ...Args>
    inline T &put(Entity entity, Args&& ...args);

    inline void remove(Entity entity) override;
    inline bool contains(Entity entity) const;

    inline T &get(Entity entity);
    inline const T &get(Entity entity) const;

    inline T *find(Entity entity);
    inline const T *find(Entity entity) const;

private:
    Map m_data;

};

} // namespace secs

#endif // SECS_COMPONENT_POOL_HPP
