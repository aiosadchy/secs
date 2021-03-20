#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

#include <utility>


template <typename...>
class View;

template <typename C>
class View<C> {
public:
    explicit View(C &collection);

    inline auto begin();
    inline auto end();

private:
    C *m_collection;

};


#define SECS_VIEW_SPECIALIZATION(base, ...)                     \
    class View<__VA_ARGS__> : public base<__VA_ARGS__> {        \
    public:                                                     \
        template <typename... Args>                             \
        explicit View(Args &&... args)                          \
            : base<__VA_ARGS__>(std::forward<Args>(args)...) {  \
        }                                                       \
    }

#endif // SECS_VIEW_HPP
