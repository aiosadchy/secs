#ifndef SECS_OBJECT_POOL_INL
#define SECS_OBJECT_POOL_INL


#include <cstddef>
#include <utility>
#include "object_pool.hpp"

template<typename T>
inline ObjectPool<T>::ObjectPool(unsigned size) : m_size(0), m_free(nullptr), m_blocks() {
    for (auto &b : m_blocks) {
        b = nullptr;
    }
    grow(size);
}

template<typename T>
inline ObjectPool<T>::~ObjectPool() {
    for (auto &b : m_blocks) {
        delete [] b;
        b = nullptr;
    }
}

template<typename T>
template<typename... Args>
inline T *ObjectPool<T>::create(Args &&... args) {
    if (m_free == nullptr) {
        if (!grow(m_size)) {
            return nullptr;
        }
    }
    Node *node = m_free;
    m_free = node->next;
    return new (&node->object) T(std::forward<Args>(args)...);
}

template<typename T>
inline void ObjectPool<T>::destroy(T *object) {
    static_assert(offsetof(Node, object) == 0);
    Node *node = reinterpret_cast<Node *>(object);
    node->object.~T();
    node->next = m_free;
    m_free = node;
}

template<typename T>
inline bool ObjectPool<T>::grow(unsigned amount) {
    Node *block = nullptr;

    for (auto &b : m_blocks) {
        if (b == nullptr) {
            b = new Node[amount];
            block = b;
            break;
        }
    }

    if (block == nullptr) {
        return false;
    } else {
        for (unsigned i = 0; i < amount - 1; ++i) {
            block[i].next = &block[i + 1];
        }
        block[amount - 1].next = m_free;
        m_free = &block[0];
        m_size += amount;
        return true;
    }
}

template<typename T>
inline ObjectPool<T>::Node::Node() {}

template<typename T>
inline ObjectPool<T>::Node::~Node() {}


#endif // SECS_OBJECT_POOL_INL
