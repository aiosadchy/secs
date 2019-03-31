#ifndef SECS_PARAMETER_PACK_HPP
#define SECS_PARAMETER_PACK_HPP


#include <type_traits>

template <typename ...Pack>
class PPack {
private:
    template <typename First>
    static constexpr bool unique() {
        return true;
    }

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool unique() {
        return !(PPack<Second, Rest...>::template contains<First>()) && unique<Second, Rest...>();
    }

    template <typename First, typename ...Rest>
    struct GetFirst {
        using Result = First;
    };

    template <typename First, typename ...Rest>
    struct GetTail {
        using Result = PPack<Rest...>;
    };

    template <typename ...>
    struct SubtractOneFrom {
        using Result = PPack<>;
    };

    template <typename T, typename First, typename ...Rest>
    struct SubtractOneFrom<T, First, Rest...> {
        using Result = typename std::conditional<std::is_same<T, First>::value, PPack<>, PPack<First>>
                            ::type::template Add<typename SubtractOneFrom<T, Rest...>::Result>::Result;
    };

public:
    template <typename T>
    static constexpr bool contains() {
        return (std::is_same<T, Pack>::value || ...);
    }

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool contains() {
        return (contains<First>() && contains<Second, Rest...>());
    }

    static constexpr bool isUnique() {
        return unique<Pack...>();
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

};

template <typename ...Types>
class PackOf;

template <typename ...Types>
class PackOf<PPack<Types...>> {
public:
    using Pointers = PPack<Types*...>;
    using References = PPack<Types&...>;
    using Constants = PPack<const Types...>;
};


#endif // SECS_PARAMETER_PACK_HPP
