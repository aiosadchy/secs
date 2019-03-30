#ifndef SECS_TYPE_SET_HPP
#define SECS_TYPE_SET_HPP


#include <type_traits>
#include <utility>

template <typename ...Types>
class TypeSet;

template <typename First, typename ...Rest>
class TypeSet<First, Rest...> : public TypeSet<Rest...> {
public:
    static_assert(ParameterPack<First, Rest...>::isUnique(), "Types in Type Set must be unique");

    inline TypeSet() : m_data() {};

    inline explicit TypeSet(First&& first, Rest&& ...rest) :
                TypeSet<Rest...>(std::forward<Rest>(rest) ...),
                m_data(std::forward<First>(first)) {}

    template <typename T>
    inline T &get() {
        static_assert(ParameterPack<First, Rest...>::template contains<T>(), "Type is not present in Type Set");
        if constexpr (std::is_same<T, First>::value) {
            return m_data;
        } else {
            return this->TypeSet<Rest...>::template get<T>();
        }
    }

    template <typename T>
    inline const T &get() const {
        return const_cast<TypeSet<First, Rest...> *>(this)->template get<T>();
    }

private:
    First m_data;

};

template <>
class TypeSet<> {};


#endif // SECS_TYPE_SET_HPP
