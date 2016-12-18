#ifndef __COOLMONKEY_TYPE_TRAITS_EXTENSIONS_H__
#define __COOLMONKEY_TYPE_TRAITS_EXTENSIONS_H__

namespace CoolMonkey
{
    namespace TypeTraits
    {
        template<typename T>
        struct AnyTypeToFalse
        {
            enum { value = false };
        };

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

#define GENERATE_CLASS_HAS_MEMBER_TYPE(type_name)                                   \
        template<typename T>                                                        \
        struct ClassHasMemberType##type_name                                        \
        {                                                                           \
            template<typename, typename> class Checker;                             \
                                                                                    \
            template<typename Class>                                                \
            static std::true_type test(Checker<Class, typename Class::type_name>*); \
                                                                                    \
            template<typename Class>                                                \
            static std::false_type test(...);                                       \
                                                                                    \
            using type = decltype(test<T>(nullptr));                                \
            enum { value = type::value };                                           \
        };
    }
}

#endif
