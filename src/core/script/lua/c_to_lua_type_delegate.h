#ifndef __COOLMONKEY_C_TO_LUA_TYPE_DELEGATE_H__
#define __COOLMONKEY_C_TO_LUA_TYPE_DELEGATE_H__

#include <type_traits>
#include <string>
#include "lua.hpp"

using std::string;

namespace CoolMonkey
{
    template<typename T, typename Enable = void>
    class CToLuaTypeDelegate
    {
    public:
        static void pushValueToLua(lua_State* lua_state, T&& value)
        {
            static_assert(false, "not implement push value to lua");
        }

        static T getValueFromLua(lua_State* lua_state)
        {
            static_assert(false, "not implement get value from lua");
        }
    };

    template<typename Boolean>
    class CToLuaTypeDelegate<Boolean, typename std::enable_if<std::is_same<typename std::decay<Boolean>::type, bool>::value>::type>
    {
    public:
        static void pushValueToLua(lua_State* lua_state, Boolean&& value)
        {
            lua_pushboolean(lua_state, static_cast<int>(value));
        }

        static Boolean getValueFromLua(lua_State* lua_state)
        {
            if (lua_isboolean(lua_state, -1))
            {
                return static_cast<Boolean>(lua_toboolean(lua_state, -1));
            }

            // error
            return Boolean();
        }
    };

    template<typename Number>
    class CToLuaTypeDelegate<Number, typename std::enable_if<std::is_arithmetic<typename std::decay<Number>::type>::value && !(std::is_same<typename std::decay<Number>::type, bool>::value)>::type>
    {
    public:
        static void pushValueToLua(lua_State* lua_state, Number&& value)
        {
            lua_pushnumber(lua_state, static_cast<lua_Number>(value));
        }

        static Number getValueFromLua(lua_State* lua_state)
        {
            if (lua_isnumber(lua_state, -1))
            {
                return static_cast<Number>(lua_tonumber(lua_state, -1));
            }

            // error
            return Number();
        }
    };

    template<typename StringType>
    class CToLuaTypeDelegate<StringType, typename std::enable_if<std::is_same<typename std::decay<StringType>::type, string>::value>::type>
    {
    public:
        static void pushValueToLua(lua_State* lua_state, StringType&& value)
        {
            lua_pushstring(lua_state, value.c_str());
        }

        static StringType getValueFromLua(lua_State* lua_state)
        {
            if (lua_isstring(lua_state, -1))
            {
                return StringType(lua_tostring(lua_state, -1));
            }

            // error
            return StringType();
        }
    };
}

#endif
