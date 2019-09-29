//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

#include <type_traits>

template <typename C>
class View {
public:
    using Begin = decltype(std::declval<C>().begin());
    using End   = decltype(std::declval<C>().end());

    explicit View(C &collection);

    Begin begin() const;
    End end() const;

private:
    Begin m_begin;
    End m_end;

};

#endif // SECS_VIEW_HPP

#ifndef SECS_VIEW_INL
#include "impl/view.inl"
#endif
