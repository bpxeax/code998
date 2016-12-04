#include "lua_context.h"
#include <iostream>
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
    }

    LuaContext::~LuaContext()
    {
        lua_close(m_lua_state);
    }
}
