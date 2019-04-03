#ifndef SECS_TYPE_ID_HPP
#define SECS_TYPE_ID_HPP


#include <functional>

#ifndef SECS_TYPEID_DEFAULT_INTERNAL_TYPE
#define SECS_TYPEID_DEFAULT_INTERNAL_TYPE unsigned short int
#endif

template <typename Family, typename IDType = SECS_TYPEID_DEFAULT_INTERNAL_TYPE>
class TypeID {
public:
    using ID = IDType;

    template <typename C>
    inline static TypeID get() {
        static const ID resultID = s_next++;
        return TypeID(resultID);
    }

    inline bool operator==(const TypeID &rhs) const {
        return m_id == rhs.m_id;
    }

    inline bool operator!=(const TypeID &rhs) const {
        return m_id != rhs.m_id;
    }

    inline bool operator<(const TypeID &rhs) const {
        return m_id < rhs.m_id;
    }

    inline ID getID() const {
        return m_id;
    }

    static const TypeID INVALID;

private:
    ID m_id;

    static typename TypeID<Family, IDType>::ID s_next;

    inline explicit TypeID(ID id) : m_id(id) {}

};

namespace std {
    template <typename Family>
    class hash<TypeID<Family>> {
    public:
        inline size_t operator()(const TypeID<Family> &typeID) const {
            return std::hash<typename TypeID<Family>::ID>()(typeID.getID());
        }
    };
}

template <typename Family, typename IDType>
typename TypeID<Family, IDType>::ID TypeID<Family, IDType>::s_next = 0;

template <typename Family, typename IDType>
const TypeID<Family, IDType> TypeID<Family, IDType>::INVALID =
        TypeID(std::numeric_limits<typename TypeID<Family, IDType>::ID>::max());


#endif // SECS_TYPE_ID_HPP
