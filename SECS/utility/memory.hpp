#ifndef SECS_MEMORY_HPP
#define SECS_MEMORY_HPP

#include "types.hpp"

class Memory {
public:
    template <typename T>
    static T *allocate(Index count);

    template <typename T>
    static void free(T *memory);

    template <typename T>
    class Storage {
    public:
        T *ptr();
        const T *ptr() const;

        T &ref();
        const T &ref() const;

    private:
        alignas(T) unsigned char m_data[sizeof(T)];

    };

};

#endif // SECS_MEMORY_HPP

#ifndef SECS_MEMORY_INL
#include "impl/memory.inl"
#endif
