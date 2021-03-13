#ifndef SECS_SPARSE_MAP_HPP
#define SECS_SPARSE_MAP_HPP

#include "SECS/collections/vector.hpp"
#include "SECS/collections/view.hpp"

template <typename T>
class SparseMap {
public:
    explicit SparseMap(Index initial_capacity, double reserve_factor = 0.5, double growth_rate = 2.0);
    ~SparseMap();

    template <typename ...Args>
    inline T &put(Index key, Args&& ...args);

    inline void remove(Index key);
    inline bool contains(Index key) const;

    inline T &get(Index key);
    inline const T &get(Index key) const;

    inline T *find(Index key);
    inline const T *find(Index key) const;

    inline Index size() const;

    View<const Vector<Index>> keys() const;

    View<Vector<T>> values();
    View<const Vector<T>> values() const;

private:
    Vector<Index> m_dense;
    Vector<Index> m_sparse;
    Vector<T> m_values;
    double m_reserve_factor;

};

#endif // SECS_SPARSE_MAP_HPP
