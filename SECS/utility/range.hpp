#ifndef SECS_RANGE_HPP
#define SECS_RANGE_HPP

template <typename T>
class Range {
public:
    class Iterator {
    public:
        inline explicit Iterator(const T &initial_value, const T &step);
        inline T &operator*();
        inline Iterator &operator++();
        inline bool operator!=(const Iterator &another) const;

    private:
        T m_value;
        T m_step;

    };

    inline explicit Range(const T &length);
    inline Range(const T &begin, const T &end, const T &step = T(1));
    inline Iterator begin() const;
    inline Iterator end() const;

private:
    T m_begin;
    T m_end;
    T m_step;

};

#endif // SECS_RANGE_HPP

#ifndef SECS_RANGE_INL
#include "impl/range.inl"
#endif
