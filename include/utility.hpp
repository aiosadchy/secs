#ifndef SECS_UTIL_HPP
#define SECS_UTIL_HPP


class Immovable {
public:
    Immovable() noexcept = default;
    ~Immovable() noexcept = default;

    Immovable(const Immovable &another) = delete;
    Immovable &operator=(const Immovable &another) = delete;

    Immovable(Immovable &&another) = delete;
    Immovable &operator=(Immovable &&another) = delete;

};


template <typename T>
class View {
public:
    using iterator = typename T::iterator;

    iterator begin() {
        return m_container->begin();
    }

    iterator end() {
        return m_container->end();
    }

    explicit View(T &container) : m_container(&container) {}

private:
    T *m_container;

};


#endif // SECS_UTIL_HPP
