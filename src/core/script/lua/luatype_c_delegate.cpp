#include "luatype_c_delegate.h"

namespace CoolMonkey
{
    LuaTable::LuaTable(const int table_ref)
        : m_table_ref(table_ref)
    {
    }

    LuaTable::LuaTable(LuaTable&& rhs_table)
    {
        m_table_ref = rhs_table.m_table_ref;
        rhs_table.m_table_ref = LUA_NOREF;
    }

    LuaTable& LuaTable::operator=(LuaTable&& rhs_table)
    {
        m_table_ref = rhs_table.m_table_ref;
        rhs_table.m_table_ref = LUA_NOREF;

        return (*this);
    }

    int LuaTable::getTableRef() const
    {
        return m_table_ref;
    }
}
