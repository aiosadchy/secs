#ifndef SECS_UTIL_HPP
#define SECS_UTIL_HPP


#include <type_traits>

template <typename ...Pack>
class ParameterPack {
public:
    template <typename T>
    static constexpr bool contains() {
        return (std::is_same<T, Pack>::value || ...);
    }

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool contains() {
        return (contains<First>() && contains<Second, Rest...>());
    }

    static constexpr bool isUnique() {
        return unique<Pack...>();
    }

private:
    template <typename First>
    static constexpr bool unique() {
        return true;
    }

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool unique() {
        return !(ParameterPack<Second, Rest...>::template contains<First>()) && unique<Second, Rest...>();
    }

};


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
