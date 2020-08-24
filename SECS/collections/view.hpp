#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

template <typename C>
class View {
public:
    explicit View(C &collection);

    inline decltype(auto) begin();
    inline decltype(auto) end();

private:
    C &m_collection;

};

#endif // SECS_VIEW_HPP

#ifndef SECS_VIEW_INL
#include "impl/view.inl"
#endif
