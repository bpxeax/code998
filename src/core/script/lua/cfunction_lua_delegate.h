#ifndef __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__
#define __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__

#include <sstream>
#include "type_traits/type_traits_extensions.h"
#include "type_traits/function_traits.h"
#include "ctype_lua_delegate.h"
#include "lua_common_operations.h"
#include "lua.hpp"

using std::ostringstream;
using std::get;

namespace CoolMonkey
{
    template<typename TFunc>
    class CToLuaFunctionDelegate
    {
    public:
        static int DelegateFunction(lua_State* lua_state)
        {
            int in_param_num = lua_gettop(lua_state);

            if (in_param_num != TypeTraits::FunctionInfo<TFunc>::arg_count)
            {
                ostringstream err_stream;
                err_stream << "lua call param count not match c funciton! " << "in: " << in_param_num << " desired: " << TypeTraits::FunctionInfo<TFunc>::arg_count << std::endl;
                lua_pushstring(lua_state, err_stream.str().c_str());
                lua_error(lua_state);

                return 0;
            }

            auto result = callFunction(lua_state, typename TypeTraits::SequenceGen<TypeTraits::FunctionInfo<TFunc>::arg_count>::type());
            lua_pop(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count));

            pushValuesToLua(lua_state, result);

            return 1;
        }

        static void addFunction(lua_State* lua_state, TFunc func, const string& func_name)
        {
            lua_pushlightuserdata(lua_state, reinterpret_cast<void*>(func));
            lua_pushcclosure(lua_state, DelegateFunction, 1);
            lua_setglobal(lua_state, func_name.c_str());
        }

    private:
        template<size_t... seq>
        static typename TypeTraits::FunctionInfo<TFunc>::ResultType callFunction(lua_State* lua_state, TypeTraits::IndexSequence<seq...>)
        {
            TFunc func_instance = reinterpret_cast<TFunc>(lua_touserdata(lua_state, lua_upvalueindex(1)));

            return func_instance(CToLuaTypeDelegate<typename TypeTraits::FunctionInfo<TFunc>::template ArgType<seq>::type>::getValueFromLua(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count - seq))...);
        }
    };

    template<typename TFunc>
    void pushCFunctionToLua(lua_State* lua_state, TFunc func, const string& func_name)
    {
        CToLuaFunctionDelegate<TFunc>::addFunction(lua_state, func, func_name);
    }
}

#endif
