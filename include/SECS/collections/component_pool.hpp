#ifndef SECS_COMPONENT_POOL_HPP
#define SECS_COMPONENT_POOL_HPP

#include "SECS/collections/sparse_map.hpp"

class AbstractComponentPool {
public:
    virtual ~AbstractComponentPool() = default;

    virtual Index size() const = 0;

protected:
    template <typename Family>
    friend class Engine;

    virtual void remove(Index key) = 0;
    virtual bool contains(Index key) const = 0;

};

template <typename T>
class ComponentPool : public AbstractComponentPool {
public:
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
    inline T &put(Index key, Args&& ...args);

    inline void remove(Index key) override;
    inline bool contains(Index key) const override;

    inline T &get(Index key);
    inline const T &get(Index key) const;

    inline T *find(Index key);
    inline const T *find(Index key) const;

    SparseMap<T> m_data;

};

#endif // SECS_COMPONENT_POOL_HPP

#include "impl/component_pool.inl"
