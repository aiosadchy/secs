#ifndef SECS_LINKED_METADATA_HPP
#define SECS_LINKED_METADATA_HPP

#include <utl/type_info.hpp>


template <typename Data, template <typename> typename Decay>
class LinkedMetadata : public utl::TypeInfo<Data, Decay, utl::init::TypeInfo::STATIC> {
public:
    using Base = utl::TypeInfo<Data, Decay, utl::init::TypeInfo::STATIC>;

    class Iterator {
    public:
        Iterator();
        explicit Iterator(const Data *item);
        const Data &operator*() const;
        const Iterator &operator++();
        bool operator!=(const Iterator &another) const;

    private:
        const Data *m_item;

    };

    class View {
    public:
        Iterator begin() const;
        Iterator end() const;

        static const Data *first();
        static const Data *next(const Data *data);
    };

    static View view();

protected:
    explicit LinkedMetadata(const Data *derived);

private:
    friend class View;

    const Data * const m_next;

    inline static const Data *s_head = nullptr;

};


#endif // SECS_LINKED_METADATA_HPP
