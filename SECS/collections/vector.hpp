//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_VECTOR_HPP
#define SECS_VECTOR_HPP

#include "SECS/utility.hpp"

template <typename T>
class Vector {
public:
    using Iterator = T *;
    using ConstIterator = const T *;

    explicit Vector(Size initial_capacity);
    Vector(const Vector &another);
    Vector(Vector &&another) noexcept;
    ~Vector();

    template <typename ...Args>
    inline T &emplace(Args&& ...args);

    inline T &append(const T &element);
    inline T &append(T &&element);
    inline void pop();

    inline T &operator[](Size index);
    inline const T &operator[](Size index) const;

    inline Size get_size() const;
    void reserve(Size count);

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

private:
    Size m_reserved;
    Size m_size;
    T *m_data;

};

#endif // SECS_VECTOR_HPP

#ifndef SECS_VECTOR_INL
#include "impl/vector.inl"
#endif
