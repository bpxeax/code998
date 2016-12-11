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
    template<typename... Args>
    void pushValuesToLua(lua_State* lua_state, Args&&... args)
    {
        CToLuaTypeDelegate<Args...>::pushValueToLua(lua_state, std::forward<Args>(args)...);
    }

    template<typename... Args>
    tuple<Args...> getValuesFromLua(lua_State* lua_state)
    {
        auto ret_result = make_tuple(CToLuaTypeDelegate<Args>::getValueFromLua(lua_state)...);

        return ret_result;
    }

    template<int...>
    struct IndexSequence {};

    template<int Num, int... Seq>
    struct SequenceGen
        : SequenceGen<Num - 1, Num - 1, Seq...>
    {};

    template<int... Seq>
    struct SequenceGen<0, Seq...>
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
                err_stream << "lua call param count not match c funciton! " << "in: " << in_param_num << "desired: " << sizeof...(Args) << std::endl;
                lua_pushstring(lua_state, err_stream.str().c_str());
                lua_error(lua_state);

                return 0;
            }

            DelegateFuncType func_instance = static_cast<DelegateFuncType>(lua_touserdata(lua_state, lua_upvalueindex(1)));

            auto args = getValuesFromLua<Args...>(lua_state);

            auto result = callFunction(func_instance, args, typename SequenceGen<sizeof...(Args)>::type());

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
        static Ret callFunction(DelegateFuncType func_instance, tuple<Args...> args, IndexSequence<seq...>)
        {
            return func_instance(get<seq>(args)...);
        }
    };
}

#endif
