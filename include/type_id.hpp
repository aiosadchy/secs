#ifndef SECS_TYPE_ID_HPP
#define SECS_TYPE_ID_HPP


#include <functional>

template <typename Family>
class TypeID {
public:
    using Index = unsigned;

    template <typename C>
    inline static TypeID get() {
        static Index resultID = s_next++;
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

    inline Index ID() const {
        return m_id;
    }

    static const TypeID INVALID;

private:
    Index m_id;

    static TypeID<Family>::Index s_next;

    inline explicit TypeID(Index id) : m_id(id) {}

};

namespace std {
    template <typename Family>
    class hash<TypeID<Family>> {
    public:
        inline size_t operator()(const TypeID<Family> &typeID) const {
            return std::hash<typename TypeID<Family>::Index>()(typeID.ID());
        }
    };
}

template <typename Family>
typename TypeID<Family>::Index TypeID<Family>::s_next = 0;

template <typename Family>
const TypeID<Family> TypeID<Family>::INVALID = TypeID(std::numeric_limits<typename TypeID<Family>::Index>::max());


#endif // SECS_TYPE_ID_HPP
