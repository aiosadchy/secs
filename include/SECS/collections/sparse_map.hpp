#ifndef SECS_SPARSE_MAP_HPP
#define SECS_SPARSE_MAP_HPP

#include <limits>
#include <vector>

#include "SECS/common.hpp"
#include "SECS/collections/view.hpp"

template <typename T>
class SparseMap {
public:
    explicit SparseMap(Index initial_capacity, double reserve_factor = 0.5);
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

    View<const std::vector<Index>> keys() const;

    View<std::vector<T>> values();
    View<const std::vector<T>> values() const;

private:
    inline static const Index NULL_VALUE = std::numeric_limits<Index>::max();

    std::vector<Index> m_dense;
    std::vector<Index> m_sparse;
    std::vector<T> m_values;
    double m_reserve_factor;

};

#endif // SECS_SPARSE_MAP_HPP
