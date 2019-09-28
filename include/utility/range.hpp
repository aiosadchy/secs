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
        inline explicit Iterator(T initial_value) :
            m_value(initial_value) {}

        inline T &operator*() {
            return m_value;
        }

        inline Iterator &operator++() {
            ++m_value;
            return *this;
        }

        inline bool operator!=(const Iterator &another) const {
            return !(m_value == another.m_value);
        }

    private:
        mutable T m_value;

    };

    inline explicit Range(const T &length) :
        m_begin(),
        m_end(length) {}

    inline explicit Range(const T &start, const T &end) :
            m_begin(start),
            m_end(end) {}

    inline Iterator begin() {
        return Iterator(m_begin);
    }

    inline Iterator end() {
        return Iterator(m_end);
    }

    inline Iterator begin() const {
        return const_cast<Range<T> *>(this)->begin();
    }

    inline Iterator end() const {
        return const_cast<Range<T> *>(this)->end();
    }

private:
    T m_begin;
    T m_end;

};

#endif // SECS_RANGE_HPP
