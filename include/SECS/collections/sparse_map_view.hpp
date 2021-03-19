#ifndef SECS_SPARSE_MAP_VIEW_HPP
#define SECS_SPARSE_MAP_VIEW_HPP

#include <vector>

#include "SECS/common.hpp"
#include "SECS/collections/view.hpp"
#include "SECS/utility.hpp"


template <typename K, typename V, typename R>
class SparseMap;


namespace detail {

template <typename M, typename T>
class SparseMapView {
protected:
    using Vector = AddConst<std::vector<T>, std::is_const_v<M>>;
    using Value = AddConst<T, std::is_const_v<M>>;

public:
    class Iterator {
    public:
        Iterator();
        Iterator(Vector &collection, Index index);

        Iterator &operator++();
        Value &operator*();
        bool operator!=(const Iterator &another) const;

    private:
        Index m_index;
        Vector *m_collection;

    };

    explicit SparseMapView(Vector &collection);

    Iterator begin();
    Iterator end();

private:
    Vector *m_collection;

};

} // namespace detail


template <typename K, typename V, typename R, typename T>
class View<SparseMap<K, V, R>, T> : public detail::SparseMapView<SparseMap<K, V, R>, T> {
private:
    using Base = detail::SparseMapView<SparseMap<K, V, R>, T>;

public:
    explicit View(typename Base::Vector &collection);

};


template <typename K, typename V, typename R, typename T>
class View<const SparseMap<K, V, R>, T> : public detail::SparseMapView<const SparseMap<K, V, R>, T> {
private:
    using Base = detail::SparseMapView<const SparseMap<K, V, R>, T>;

public:
    explicit View(typename Base::Vector &collection);

};



#endif // SECS_SPARSE_MAP_VIEW_HPP
