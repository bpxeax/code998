#ifndef __COOLMONKEY_LUAFUNCTION_C_DELEGATE_H__
#define __COOLMONKEY_LUAFUNCTION_C_DELEGATE_H__

#include <string>
#include "lua_context.h"
#include "lua.hpp"

using std::string;

namespace CoolMonkey
{
    class LuaContext;
    class LuaTable;

    template<typename R, typename... Args>
    class LuaFunctionCDelegate
    {
    public:
        LuaFunctionCDelegate(LuaContext* lua_context, const string func_name)
            : m_lua_context(lua_context)
            , m_table(lua_context->getGlobalTable())
        {
            
        }

        LuaFunctionCDelegate(LuaContext* lua_context, const string func_name, LuaTable& table)
            : m_lua_context(lua_context)
            , m_table(table)
        {
            
        }

        R operator()(Args&&...)
        {
            return R();
        }

    private:
        LuaContext* m_lua_context{ nullptr };
        LuaTable& m_table;
        int m_func_ref{ LUA_NOREF };
    };
}

#endif
