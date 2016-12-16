#ifndef __COOLMONKEY_TYPE_TRAITS_EXTENSIONS_H__
#define __COOLMONKEY_TYPE_TRAITS_EXTENSIONS_H__

namespace CoolMonkey
{
    namespace TypeTraits
    {
        template<size_t N, typename THead, typename... TTails>
        struct VariadicParameterExtractor
        {
            static_assert(N < 1 + sizeof...(TTails), "out of type index");
            using type = typename VariadicParameterExtractor<N - 1, TTails...>::type;
        };

        template<typename THead, typename... TTails>
        struct VariadicParameterExtractor<0, THead, TTails...>
        {
            using type = THead;
        };

        template<size_t...>
        struct IndexSequence {};

        template<size_t Num, size_t... Seq>
        struct SequenceGen
            : SequenceGen<Num - 1, Num - 1, Seq...>
        {};

        template<size_t... Seq>
        struct SequenceGen<0, Seq...>
        {
            typedef IndexSequence<Seq...> type;
        };

        template<size_t Num, size_t... Seq>
        struct ReverseSequenceGen
            : ReverseSequenceGen<Num - 1, Seq..., Num - 1>
        {};

        template<size_t... Seq>
        struct ReverseSequenceGen<0, Seq...>
        {
            typedef IndexSequence<Seq...> type;
        };
    }
}

#endif
