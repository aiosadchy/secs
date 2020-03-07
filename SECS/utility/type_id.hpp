#ifndef SECS_TYPE_ID_HPP
#define SECS_TYPE_ID_HPP

template <typename Family, typename IDType = unsigned short int>
class TypeID {
public:
    using ID = IDType;

    TypeID();
    ID get_id() const;

    template <typename T>
    static TypeID get();

    bool operator==(const TypeID &another) const;
    bool operator!=(const TypeID &another) const;

private:
    explicit TypeID(ID id);

    ID m_id;
    static ID s_next_type_id;

};

#ifndef SECS_TYPE_ID_INL
#include "impl/type_id.inl"
#endif

#endif // SECS_TYPE_ID_HPP
