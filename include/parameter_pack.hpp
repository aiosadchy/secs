#ifndef SECS_PARAMETER_PACK_HPP
#define SECS_PARAMETER_PACK_HPP


#include <type_traits>

template <typename ...Pack>
class PPack {
private:
    template <typename T, bool present>
    struct AddIfNotPresent {
        using Result = PPack<Pack...>;
    };

    template <typename T>
    struct AddIfNotPresent<T, false> {
        using Result = PPack<Pack..., T>;
    };

    template <typename First, typename ...Rest>
    struct GetFirst {
        using Result = First;
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
        using Result = typename AddIfNotPresent<T, contains<T>()>::Result;
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
        using Result = typename PPack<typename GetFirst<Pack...>::Result>::template AddUnique<Pack...>::Result;
    };

private:
    template <typename First>
    static constexpr bool unique() {
        return true;
    }

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool unique() {
        return !(PPack<Second, Rest...>::template contains<First>()) && unique<Second, Rest...>();
    }

};


#endif // SECS_PARAMETER_PACK_HPP
