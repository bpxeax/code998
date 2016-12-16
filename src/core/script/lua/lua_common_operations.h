#ifndef __COOLMONKEY_LUA_COMMON_OPERATIONS_H__
#define __COOLMONKEY_LUA_COMMON_OPERATIONS_H__

#include "ctype_lua_delegate.h"

struct lua_State;

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
        pushValuesToLua(lua_state, std::forward<THead>(value));
        pushValuesToLua(lua_state, std::forward<TTails>(args)...);
    }
}

#endif

