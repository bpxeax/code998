#ifndef __COOLMONKEY_LUATYPE_C_DELEGATE_H__
#define __COOLMONKEY_LUATYPE_C_DELEGATE_H__

#include "lua.hpp"

namespace CoolMonkey
{
    class LuaContext;

    class LuaTableInstance
    {
    public:
        LuaTableInstance();
        explicit LuaTableInstance(lua_State* lua_state, const int value_index = 1);
        LuaTableInstance(const LuaTableInstance& rhs_table_instance);
        LuaTableInstance& operator=(const LuaTableInstance& rhs_table_instance);
        LuaTableInstance(LuaTableInstance&& rhs_table_instance) noexcept;
        LuaTableInstance& operator=(LuaTableInstance&& rhs_table_instance) noexcept;
        ~LuaTableInstance();

        int getValueToStack() const;

    private:
        lua_State* m_lua_state{ nullptr };
        int m_table_ref{ LUA_NOREF };
        unsigned int* m_ref_count{ nullptr };
    };
}

#endif
