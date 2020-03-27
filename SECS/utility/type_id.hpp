#ifndef SECS_TYPE_ID_HPP
#define SECS_TYPE_ID_HPP

template <typename Family, typename Index = unsigned short int>
class TypeID {
public:
    TypeID();
    Index index() const;

    bool operator==(const TypeID &another) const;
    bool operator!=(const TypeID &another) const;

    template <typename T>
    static TypeID get();

    static Index family_size();

private:
    template <typename T>
    class Initializer {
    public:
        const static Index s_index;
    };

    explicit TypeID(const Index &id);

    template <typename T>
    static Index get_index();

    Index m_index;
    static Index s_family_size;

};

#ifndef SECS_TYPE_ID_INL
#include "impl/type_id.inl"
#endif

#endif // SECS_TYPE_ID_HPP
