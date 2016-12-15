#include <iostream>
#include "lua_context.h"
#include "luatype_c_delegate.h"
#include "lua.hpp"

namespace CoolMonkey
{
    void LuaContext::executeFile(string file_name)
    {
        if (luaL_dofile(m_lua_state, file_name.c_str()))
        {
            // execute file error
            const char* err_str = lua_tostring(m_lua_state, -1);
            std::cerr << "execution script fail: " << err_str << std::endl;
        }
    }

    void LuaContext::executeString(string content)
    {
        if (luaL_dostring(m_lua_state, content.c_str()))
        {
            // execute content error
            const char* err_str = lua_tostring(m_lua_state, -1);
            std::cerr << "execution script fail: " << err_str << std::endl;
        }
    }

    LuaContext::LuaContext()
    {
        m_lua_state = luaL_newstate();
        luaL_openlibs(m_lua_state);

        lua_getglobal(m_lua_state, "_G");
        int global_table_ref = lua_ref(m_lua_state, 1);
        m_global_table = new LuaTable(global_table_ref);
    }

    LuaContext::~LuaContext()
    {
        lua_unref(m_lua_state, m_global_table->getTableRef());
        delete m_global_table;
        m_global_table = nullptr;
        lua_close(m_lua_state);
    }

    lua_State* LuaContext::getLuaStateInstance()
    {
        return m_lua_state;
    }

    LuaTable& LuaContext::getGlobalTable()
    {
        return *m_global_table;
    }
}
