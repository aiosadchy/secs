#ifndef SECS_MEMORY_HPP
#define SECS_MEMORY_HPP

#include "types.hpp"

class Memory {
public:
    template <typename T>
    static T *allocate(Size count);

    template <typename T>
    static void free(T *memory);

    template <typename T>
    struct Placeholder {
    public:
        Placeholder();

        T *as_object();
        const T *as_object() const;

    private:
        alignas(T) unsigned char m_data[sizeof(T)];

    };

};

#endif // SECS_MEMORY_HPP

#ifndef SECS_MEMORY_INL
#include "impl/memory.inl"
#endif
