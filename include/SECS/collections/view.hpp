#ifndef SECS_VIEW_HPP
#define SECS_VIEW_HPP

template <typename...>
class View;

template <typename C>
class View<C> {
public:
    explicit View(C &collection);

    inline auto begin();
    inline auto end();

    inline auto begin() const;
    inline auto end() const;

private:
    C *m_collection;

};

#endif // SECS_VIEW_HPP
