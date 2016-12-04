#ifndef __COOLMONKEY_C_TO_LUA_DELEGATE_H__
#define __COOLMONKEY_C_TO_LUA_DELEGATE_H__

#include <sstream>
#include "c_to_lua_type_delegate.h"
#include "lua.hpp"

using std::ostringstream;

namespace CoolMonkey
{
    template<typename T>
    void pushArgsToLua(T&& arg)
    {
        CToLuaTypeDelegate<T>::pushValueToLua(std::forward<T>(arg));
    }

    template<typename THead, typename... TTails>
    void pushArgsToLua(THead&& head_arg, TTails&&... tail_args)
    {
        CToLuaTypeDelegate<THead>::pushValueToLua(std::forward<THead>(head_arg));
        pushArgsToLua(std::forward<TTails>(tail_args)...);
    }

    template<typename Ret, typename... Args>
    class CToLuaFunctionDelegate
    {
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



            return 1;
        }
    };
}

#endif
