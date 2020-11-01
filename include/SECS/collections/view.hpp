#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

template <typename C>
class View {
public:
    explicit View(C &collection);

    inline auto begin();
    inline auto end();

private:
    C &m_collection;

};

#endif // SECS_VIEW_HPP

#include "impl/view.inl"
