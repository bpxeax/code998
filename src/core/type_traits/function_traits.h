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

        template<typename TFunc>
        class FunctionPointer
        {
            static_assert(std::is_function<typename std::remove_pointer<TFunc>::type>::value || std::is_member_function_pointer<TFunc>::value, "the type must be function pointer!");

        public:
            explicit FunctionPointer(TFunc func_ptr)
                : m_func_ptr(func_ptr)
            {

            }

            explicit FunctionPointer(void* raw_ptr)
                : m_raw_ptr(raw_ptr)
            {

            }

            explicit operator void*() const
            {
                return m_raw_ptr;
            }

            explicit operator TFunc() const
            {
                return m_func_ptr;
            }

        private:
            union
            {
                TFunc m_func_ptr;
                void* m_raw_ptr;
            };
        };
    }
}

#endif
