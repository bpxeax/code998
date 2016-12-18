#ifndef __COOLMONKEY_FUNCTION_TRAITS_H__
#define __COOLMONKEY_FUNCTION_TRAITS_H__

#include "type_traits_extensions.h"

namespace CoolMonkey
{
    namespace TypeTraits
    {
        template<typename T>
        class FunctionInfo;

        template<typename R, typename... Args>
        class FunctionInfo<R(*)(Args...)>
        {
        public:
            using ResultType = R;
            
            enum : size_t { arg_count = sizeof...(Args) };

            template<size_t type_index>
            struct ArgType
            {
                using type = typename TypeTraits::VariadicParameterExtractor<type_index, Args...>::type;
            };
        };

        template<typename C, typename R, typename... Args>
        class FunctionInfo<R(C::*)(Args...)>
        {
        public:
            using ResultType = R;
            using ClassType = C;

            enum : size_t { arg_count = sizeof...(Args) };

            template<size_t type_index>
            struct ArgType
            {
                using type = typename TypeTraits::VariadicParameterExtractor<type_index, Args...>::type;
            };
        };
    }
}

#endif
