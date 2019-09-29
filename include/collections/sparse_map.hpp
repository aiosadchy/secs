//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_SPARSE_MAP_HPP
#define SECS_SPARSE_MAP_HPP

#include "vector.hpp"
#include "view.hpp"

template <typename T>
class SparseMap {
public:
    explicit SparseMap(Size initial_capacity);
    ~SparseMap();

    template <typename ...Args>
    T &put(Size key, Args&& ...args);

    void remove(Size key);
    bool contains(Size key) const;

    T &operator[](Size key);
    const T &operator[](Size key) const;

    Size get_size() const;

    View<const Vector<Size>> get_keys() const;

    View<Vector<T>> get_values();
    View<const Vector<T>> get_values() const;

private:
    Vector<Size> m_dense;
    Vector<Size> m_sparse;
    Vector<T> m_values;

};

#endif // SECS_SPARSE_MAP_HPP

#ifndef SECS_SPARSE_MAP_INL
#include "impl/sparse_map.inl"
#endif
