#ifndef SECS_SPARSE_MAP_HPP
#define SECS_SPARSE_MAP_HPP

#include <limits>
#include <vector>

#include "SECS/common.hpp"
#include "SECS/collections/sparse_map_view.hpp"
#include "SECS/utility.hpp"


namespace secs {

template <typename K, typename V, typename R = Identity>
class SparseMap {
public:
    explicit SparseMap(Index initial_capacity = 0, R key_reduce = R());
    ~SparseMap();

    template <typename ...Args>
    inline V &put(const K &key, Args&& ...args);

    inline void remove(const K &key);
    inline bool contains(const K &key) const;

    inline V &get(const K &key);
    inline const V &get(const K &key) const;

    inline V *find(const K &key);
    inline const V *find(const K &key) const;

    inline Index size() const;

    View<const SparseMap<K, V, R>, K> keys() const;

    View<SparseMap<K, V, R>, V> values();
    View<const SparseMap<K, V, R>, V> values() const;

private:
    inline static const Index NULL_VALUE = std::numeric_limits<Index>::max();

    std::vector<K> m_dense;
    std::vector<Index> m_sparse;
    std::vector<V> m_values;
    R m_key_reduce;

};

} // namespace secs

#endif // SECS_SPARSE_MAP_HPP
