#ifndef SECS_OBJECT_POOL_HPP
#define SECS_OBJECT_POOL_HPP


#ifndef SECS_OBJECT_POOL_MAX_BLOCKS
#define SECS_OBJECT_POOL_MAX_BLOCKS 30
#endif // SECS_OBJECT_POOL_MAX_BLOCKS

#include "utility.hpp"

template <typename T>
class ObjectPool : public Immovable {
public:
    explicit ObjectPool(unsigned size);
    ~ObjectPool();

    template <typename... Args>
    T *create(Args&& ...args);

    void destroy(T *object);

private:
    union Node {
        Node();
        ~Node();
        T object;
        Node *next;
    };

    bool grow(unsigned amount);

    unsigned m_size;
    Node *m_free;
    Node *m_blocks[SECS_OBJECT_POOL_MAX_BLOCKS];
};


#endif // SECS_OBJECT_POOL_HPP
