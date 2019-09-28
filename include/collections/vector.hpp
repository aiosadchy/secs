//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_VECTOR_HPP
#define SECS_VECTOR_HPP

#include "../utility.hpp"

template <typename T>
class Vector {
public:
    explicit Vector(Size initial_capacity);
    ~Vector();

    template <typename ...Args>
    T &emplace(Args&& ...args);

    T &append(const T &element);
    T &append(T &&element);
    void pop();

    inline T &operator[](Size index);
    inline const T &operator[](Size index) const;

    Size get_size() const;

    T *begin();
    T *end();
    const T *begin() const;
    const T *end() const;

private:
    Size m_reserved;
    Size m_size;
    T *m_data;

    void reserve(Size new_size);

    static void free(T *data);

};

#endif // SECS_VECTOR_HPP

#include "impl/vector.inl"
