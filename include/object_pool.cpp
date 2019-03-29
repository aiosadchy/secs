#include <cstddef>
#include <utility>
#include "object_pool.hpp"

template<typename T>
ObjectPool<T>::ObjectPool(unsigned size) : m_blocks(nullptr), m_free(nullptr), m_size(0) {
    grow(size);
}

template<typename T>
ObjectPool<T>::~ObjectPool() {
    while (m_blocks != nullptr) {
        Node *next = m_blocks->nextBlock;
        delete [] m_blocks;
        m_blocks = next;
    }
}

template<typename T>
template<typename... Args>
T *ObjectPool<T>::create(Args &&... args) {
    if (m_free == nullptr) {
        grow(m_size);
    }
    Node *node = m_free;
    m_free = node->nextFree;
    return new (&node->object) T(std::forward<Args>(args)...);
}

template<typename T>
void ObjectPool<T>::destroy(T *object) {
    static_assert(offsetof(Node, object) == 0, "Offset of first element of a union is nonzero");
    Node *node = reinterpret_cast<Node *>(object);
    node->object.~T();
    node->nextFree = m_free;
    m_free = node;
}

template<typename T>
void ObjectPool<T>::grow(unsigned amount) {
    Node *block = new Node[amount + 1];

    block->nextBlock = m_blocks;
    m_blocks = block;

    for (unsigned i = 1; i < amount; ++i) {
        block[i].nextFree = &block[i + 1];
    }
    block[amount].nextFree = m_free;
    m_free = &block[1];

    m_size += amount;
}

template<typename T>
ObjectPool<T>::Node::Node() {}

template<typename T>
ObjectPool<T>::Node::~Node() {}
