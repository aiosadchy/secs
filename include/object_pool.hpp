#ifndef SECS_OBJECT_POOL_HPP
#define SECS_OBJECT_POOL_HPP


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
        Node *nextFree;
        Node *nextBlock;
    };

    void grow(unsigned amount);

    Node *m_blocks;
    Node *m_free;
    unsigned m_size;
};


#endif // SECS_OBJECT_POOL_HPP
