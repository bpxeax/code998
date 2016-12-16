#ifndef __COOLMONKEY_LUAFUNCTION_C_DELEGATE_H__
#define __COOLMONKEY_LUAFUNCTION_C_DELEGATE_H__

#include <string>
#include "luatype_c_delegate.h"
#include "lua_common_operations.h"
#include "lua.hpp"

using std::string;

namespace CoolMonkey
{
    class LuaTableInstance;

    class LuaFunctionCDelegate
    {
    public:
        LuaFunctionCDelegate(lua_State* lua_state, const string func_name)
            : m_lua_state(lua_state)
            , m_table()
        {
            lua_getglobal(m_lua_state, func_name.c_str());
            m_func_ref = lua_ref(m_lua_state, 1);
        }

        LuaFunctionCDelegate(lua_State* lua_state, const string func_name, const LuaTableInstance& table)
            : m_lua_state(lua_state)
            , m_table(table)
        {
            m_table.getValueToStack();
            lua_getfield(m_lua_state, -1, func_name.c_str());
            m_func_ref = lua_ref(m_lua_state, 1);
            lua_pop(m_lua_state, 1);
        }

        template<typename R, typename... Args>
        typename std::enable_if<std::is_same<R, void>::value>::type invoke(Args&&... args)
        {
            lua_getref(m_lua_state, m_func_ref);
            int table_count = m_table.getValueToStack();
            pushValuesToLua(m_lua_state, std::forward<Args>(args)...);

            if (lua_pcall(m_lua_state, static_cast<int>(table_count + sizeof...(Args)), 0, 0))
            {
                string err_msg = lua_tostring(m_lua_state, -1);
                lua_pop(m_lua_state, 1);
                std::cerr << "c call lua error: " << err_msg << std::endl;

                return;
            }
        }

        template<typename R, typename... Args>
        typename std::enable_if<!std::is_same<R, void>::value, R>::type invoke(Args&&... args)
        {
            lua_getref(m_lua_state, m_func_ref);
            int table_count = m_table.getValueToStack();
            pushValuesToLua(m_lua_state, std::forward<Args>(args)...);

            if (lua_pcall(m_lua_state, static_cast<int>(table_count + sizeof...(Args)), 1, 0))
            {
                string err_msg = lua_tostring(m_lua_state, -1);
                lua_pop(m_lua_state, 1);
                std::cerr << "c call lua error: " << err_msg << std::endl;

                return R();
            }

            R ret_value(CToLuaTypeDelegate<R>::getValueFromLua(m_lua_state, 1));

            lua_pop(m_lua_state, 1);

            return ret_value;
        }

    private:
        lua_State* m_lua_state{ nullptr };
        LuaTableInstance m_table;
        int m_func_ref{ LUA_NOREF };
    };
}

#endif
