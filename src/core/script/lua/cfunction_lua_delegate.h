#ifndef __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__
#define __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__

#include <sstream>
#include "type_traits/type_traits_extensions.h"
#include "ctype_lua_delegate.h"
#include "lua_common_operations.h"
#include "lua.hpp"

using std::ostringstream;
using std::get;

namespace CoolMonkey
{
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

            auto result = callFunction(lua_state, typename TypeTraits::SequenceGen<sizeof...(Args)>::type());
            lua_pop(lua_state, static_cast<int>(sizeof...(Args)));

            pushValuesToLua(lua_state, result);

            return 1;
        }

        static void addFunction(lua_State* lua_state, Ret(*func)(Args...), string func_name)
        {
            lua_pushlightuserdata(lua_state, reinterpret_cast<void*>(func));
            lua_pushcclosure(lua_state, DelegateFunction, 1);
            lua_setglobal(lua_state, func_name.c_str());
        }

    private:
        template<size_t... seq>
        static Ret callFunction(lua_State* lua_state, TypeTraits::IndexSequence<seq...>)
        {
            DelegateFuncType func_instance = reinterpret_cast<DelegateFuncType>(lua_touserdata(lua_state, lua_upvalueindex(1)));

            return func_instance(CToLuaTypeDelegate<typename TypeTraits::VariadicParameterExtractor<seq, Args...>::type>::getValueFromLua(lua_state, static_cast<int>(sizeof...(Args)-seq))...);
        }
    };
}

#endif
