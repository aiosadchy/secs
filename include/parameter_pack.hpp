#ifndef SECS_PARAMETER_PACK_HPP
#define SECS_PARAMETER_PACK_HPP


#include <type_traits>

template <typename ...Pack>
class PPack {
private:
    PPack() = default;

    template <typename First, typename ...Rest>
    static constexpr bool unique() {
        if constexpr (sizeof...(Rest) != 0) {
            return !(PPack<Rest...>::template contains<First>()) && unique<Rest...>();
        } else {
            return true;
        }
    }

    template <typename ...>
    struct GetFirst {
        using Result = PPack<>;
    };

    template <typename First, typename ...Rest>
    struct GetFirst<First, Rest...> {
        using Result = First;
    };

    template <typename ...>
    struct GetTail {
        using Result = PPack<>;
    };

    template <typename First, typename ...Rest>
    struct GetTail<First, Rest...> {
        using Result = PPack<Rest...>;
    };

    template <typename T, typename ...>
    struct SubtractOneFrom {
        using Result = PPack<>;
    };

    template <typename T, typename First, typename ...Rest>
    struct SubtractOneFrom<T, First, Rest...> {
        using Head = typename std::conditional<std::is_same<T, First>::value, PPack<>, PPack<First>>::type;
        using Result = typename Head::template Add<typename SubtractOneFrom<T, Rest...>::Result>::Result;
    };

public:
    template <typename First, typename ...Rest>
    static constexpr bool contains() {
        if constexpr (sizeof...(Rest) != 0) {
            return (contains<First>() && contains<Rest...>());
        } else {
            return (std::is_same<First, Pack>::value || ...);
        }
    }

    static constexpr bool isUnique() {
        if constexpr (size() != 0) {
            return unique<Pack...>();
        } else {
            return true;
        }
    }

    static constexpr decltype(sizeof...(Pack)) size() {
        return sizeof...(Pack);
    }

    struct First {
        using Result = typename GetFirst<Pack...>::Result;
    };

    struct Tail {
        using Result = typename GetTail<Pack...>::Result;
    };

    template <typename ...Other>
    struct Add {
        using Result = PPack<Pack..., Other...>;
    };

    template <typename ...Other>
    struct Add<PPack<Other...>> {
        using Result = PPack<Pack..., Other...>;
    };

    template <typename ...Other>
    struct AddUnique {
        using Result = PPack<Pack...>;
    };

    template <typename T>
    struct AddUnique<T> {
        using Result = typename std::conditional<contains<T>(), PPack<Pack...>, PPack<Pack..., T>>::type;
    };

    template <typename First, typename Second, typename ...Rest>
    struct AddUnique<First, Second, Rest...> {
        using Result = typename AddUnique<First>::Result::template AddUnique<Second, Rest...>::Result;
    };

    template <typename ...Other>
    struct AddUnique<PPack<Other...>> {
        using Result = typename AddUnique<Other...>::Result;
    };

    struct Unique {
        using Result = typename PPack<typename First::Result>::template AddUnique<typename Tail::Result>::Result;
    };

    template <typename ...Other>
    struct Subtract {
        using Result = PPack<Pack...>;
    };

    template <typename First, typename ...Other>
    struct Subtract<First, Other...> {
        using Result = typename SubtractOneFrom<First, Pack...>::Result::template Subtract<Other...>::Result;
    };

    template <typename ...Types, typename ...Other>
    struct Subtract<PPack<Types...>, Other...> {
        using Result = typename Subtract<Types..., Other...>::Result;
    };

    inline static PPack<Pack...> instance() {
        return PPack<Pack...>();
    }
};


#endif // SECS_PARAMETER_PACK_HPP
