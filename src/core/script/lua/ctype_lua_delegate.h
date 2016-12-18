#ifndef __COOLMONKEY_CTYPE_LUA_DELEGATE_H__
#define __COOLMONKEY_CTYPE_LUA_DELEGATE_H__

#include <type_traits>
#include "type_traits/type_traits_extensions.h"
#include "luatype_c_delegate.h"
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
            static_assert(TypeTraits::AnyTypeToFalse<T>::value, "not implement push value to lua");
        }

        static T getValueFromLua(lua_State* lua_state, int value_index = 1)
        {
            static_assert(TypeTraits::AnyTypeToFalse<T>::value, "not implement get value from lua");
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

        static Boolean getValueFromLua(lua_State* lua_state, int value_index = 1)
        {
            if (lua_isboolean(lua_state, -value_index))
            {
                return static_cast<Boolean>(lua_toboolean(lua_state, -value_index));
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

        static Number getValueFromLua(lua_State* lua_state, int value_index = 1)
        {
            if (lua_isnumber(lua_state, -value_index))
            {
                return static_cast<Number>(lua_tonumber(lua_state, -value_index));
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

        static StringType getValueFromLua(lua_State* lua_state, int value_index = 1)
        {
            if (lua_isstring(lua_state, -value_index))
            {
                return StringType(lua_tostring(lua_state, -value_index));
            }

            // error
            return StringType();
        }
    };

    template<typename LuaTableType>
    class CToLuaTypeDelegate<LuaTableType, typename std::enable_if<std::is_same<typename std::decay<LuaTableType>::type, LuaTableInstance>::value>::type>
    {
    public:
        static void pushValueToLua(lua_State* lua_state, LuaTableType&& value)
        {
            value.getValueToStack();
        }

        static LuaTableType getValueFromLua(lua_State* lua_state, int value_index = 1)
        {
            return LuaTableType(lua_state, value_index);
        }
    };
}

#endif
