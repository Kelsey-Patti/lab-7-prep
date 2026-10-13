#ifndef PACK_H
#define PACK_H
#include <type_traits>
namespace pack_detail{
    // packsize
    template <typename... Args> 
    constexpr int packSize = sizeof...(Args);

    // _packIndexToType
    template <int QueryIndex, typename... Args>
    struct _packIndexToType;


    template <int QueryIndex, typename First, typename... Rest>
    struct _packIndexToType<QueryIndex, First, Rest...>{
        static_assert(QueryIndex >= 0);
        static_assert(QueryIndex < 1 +sizeof...(Rest));
        using type = typename _packIndexToType<QueryIndex -1, Rest...>::type; //chatGPT: recursive help
    };

    template <typename First, typename... Rest>
    struct _packIndexToType<0, First, Rest...>{
        using type = First;
    };


    // packIndexToType Alias
    template <int QueryIndex, typename... Args>
    using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

    //packTypeToIndex
    template <int Accumulated, typename QueryType, typename... Args>
    constexpr int _packTypeToIndex = -1;

    template <int Accumulated, typename QueryType, typename First, typename... Rest>
    constexpr int _packTypeToIndex<Accumulated, QueryType, First, Rest...> = std::is_same_v<QueryType, First> ?Accumulated : _packTypeToIndex<Accumulated +1, QueryType, Rest...>; //chatGPT helped come up with the ?accumulated synatx

    template <typename QueryType, typename... Args>
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;
}

template <typename... Args>
struct Pack {
    static constexpr int size = pack_detail::packSize<Args...>;

    template <int QueryIndex>
    using type = typename pack_detail::_packIndexToType<QueryIndex, Args...>::type;

    template <typename QueryType> 
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H
