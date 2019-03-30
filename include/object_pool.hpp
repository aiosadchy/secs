#ifndef SECS_OBJECT_POOL_HPP
#define SECS_OBJECT_POOL_HPP


#ifndef SECS_OBJECT_POOL_MAX_BLOCKS
#define SECS_OBJECT_POOL_MAX_BLOCKS 30
#endif // SECS_OBJECT_POOL_MAX_BLOCKS

#include "utility.hpp"

template <typename T>
class ObjectPool : public Immovable {
public:
    inline explicit ObjectPool(unsigned size);
    inline ~ObjectPool();

    template <typename... Args>
    inline T *create(Args&& ...args);

    inline void destroy(T *object);

private:
    union Node {
        inline Node();
        inline ~Node();
        T object;
        Node *next;
    };

    inline bool grow(unsigned amount);

    unsigned m_size;
    Node *m_free;
    Node *m_blocks[SECS_OBJECT_POOL_MAX_BLOCKS];
};


#endif // SECS_OBJECT_POOL_HPP

#include "object_pool.inl"

