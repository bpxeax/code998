#ifndef __COOLMONKEY_C_TO_LUA_DELEGATE_H__
#define __COOLMONKEY_C_TO_LUA_DELEGATE_H__

#include <sstream>
#include <functional>
#include "c_to_lua_type_delegate.h"
#include "lua.hpp"

using std::ostringstream;
using std::string;
using std::function;
using std::tuple;
using std::make_tuple;
using std::tuple_cat;
using std::get;

namespace CoolMonkey
{
    template<typename T>
    void pushValuesToLua(lua_State* lua_state, T&& value)
    {
        CToLuaTypeDelegate<T>::pushValueToLua(lua_state, std::forward<T>(value));
    }

    template<typename THead, typename... TTails>
    void pushValuesToLua(lua_State* lua_state, THead&& value, TTails&&... args)
    {
        CToLuaTypeDelegate<THead>::pushValueToLua(lua_state, std::forward<THead>(value));
        pushValuesToLua(lua_state, std::forward<TTails>(args)...);
    }

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

    template<size_t N, typename... Args>
    typename VariadicParameterExtractor<N, Args...>::type getValuesFromLua(lua_State* lua_state)
    {
        return CToLuaTypeDelegate<typename VariadicParameterExtractor<N, Args...>::type>::getValueFromLua(lua_state, static_cast<int>(sizeof...(Args) - N));
    }

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

    template<typename Ret, typename... Args>
    class CToLuaFunctionDelegate
    {
    public:
        typedef Ret(*DelegateFuncType)(Args...);

    public:
        static int DelegateFunction(lua_State* lua_state)
        {
            int in_param_num = lua_gettop(lua_state);

            if (in_param_num != sizeof...(Args))
            {
                ostringstream err_stream;
                err_stream << "lua call param count not match c funciton! " << "in: " << in_param_num << " desired: " << sizeof...(Args) << std::endl;
                lua_pushstring(lua_state, err_stream.str().c_str());
                lua_error(lua_state);

                return 0;
            }

            auto result = callFunction(lua_state, typename SequenceGen<sizeof...(Args)>::type());
            lua_pop(lua_state, static_cast<int>(sizeof...(Args)));

            pushValuesToLua(lua_state, result);

            return 1;
        }

        static void addFunction(lua_State* lua_state, Ret(*func)(Args...), string func_name)
        {
            lua_pushlightuserdata(lua_state, func);
            lua_pushcclosure(lua_state, DelegateFunction, 1);
            lua_setglobal(lua_state, func_name.c_str());
        }

    private:
        template<int... seq>
        static Ret callFunction(lua_State* lua_state, IndexSequence<seq...>)
        {
            DelegateFuncType func_instance = static_cast<DelegateFuncType>(lua_touserdata(lua_state, lua_upvalueindex(1)));
            return func_instance(getValuesFromLua<seq, Args...>(lua_state)...);
        }
    };
}

#endif
