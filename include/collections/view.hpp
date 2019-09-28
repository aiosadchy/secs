//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

template <typename Iterator>
class View {
public:
    template <typename B, typename E>
    View(B &&begin, E &&end);

    Iterator begin() const;
    Iterator end() const;

private:
    Iterator m_begin;
    Iterator m_end;

};

#endif // SECS_VIEW_HPP

#include "impl/view.inl"
