//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_RANGE_HPP
#define SECS_RANGE_HPP

template <typename T = int>
class Range {
public:
    class Iterator {
    public:
        inline explicit Iterator(T initial_value);
        inline T &operator*();
        inline Iterator &operator++();
        inline bool operator!=(const Iterator &another) const;

    private:
        T m_value;

    };

    inline explicit Range(const T &length);
    inline Range(const T &start, const T &end);
    inline Iterator begin();
    inline Iterator end();
    inline Iterator begin() const;
    inline Iterator end() const;

private:
    T m_begin;
    T m_end;

};

#endif // SECS_RANGE_HPP

#ifndef SECS_RANGE_INL
#include "impl/range.inl"
#endif
