#ifndef SECS_COMPONENT_POOL_HPP
#define SECS_COMPONENT_POOL_HPP

#include "SECS/collections/sparse_map.hpp"
#include "SECS/entity.hpp"

class AbstractComponentPool {
public:
    virtual ~AbstractComponentPool() = default;

    virtual Index size() const = 0;

protected:
    template <typename Family>
    friend class Engine;

    virtual void remove(Entity::ID key) = 0;
    virtual bool contains(Entity::ID key) const = 0;

};

template <typename T>
class ComponentPool : public AbstractComponentPool {
public:
    // TODO: create iterator class since it is used as std::variant element in EntityView

    explicit ComponentPool(Index initial_capacity);
    ~ComponentPool() override = default;

    inline auto begin();
    inline auto begin() const;

    inline auto end();
    inline auto end() const;

    inline Index size() const override;

private:
    template <typename Family>
    friend class Engine;

    template <typename ...Args>
    inline T &put(Entity::ID key, Args&& ...args);

    inline void remove(Entity::ID key) override;
    inline bool contains(Entity::ID key) const override;

    inline T &get(Entity::ID key);
    inline const T &get(Entity::ID key) const;

    inline T *find(Entity::ID key);
    inline const T *find(Entity::ID key) const;

    SparseMap<T> m_data;

};

#endif // SECS_COMPONENT_POOL_HPP
