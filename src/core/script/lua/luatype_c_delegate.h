#ifndef __COOLMONKEY_LUATYPE_C_DELEGATE_H__
#define __COOLMONKEY_LUATYPE_C_DELEGATE_H__

#include "lua.hpp"

namespace CoolMonkey
{
    class LuaTable
    {
    public:
        explicit LuaTable(const int table_ref = LUA_NOREF);
        LuaTable(const LuaTable&) = delete;
        LuaTable& operator=(const LuaTable&) = delete;
        LuaTable(LuaTable&& rhs_table);
        LuaTable& operator=(LuaTable&& rhs_table);

        int getTableRef() const;

    private:
        int m_table_ref{ LUA_NOREF };
    };
}

#endif
