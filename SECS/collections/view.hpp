//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

template <typename C>
class View {
public:
    explicit View(C &collection);

    auto begin();
    auto end();

private:
    C &m_collection;

};

#endif // SECS_VIEW_HPP

#ifndef SECS_VIEW_INL
#include "impl/view.inl"
#endif
