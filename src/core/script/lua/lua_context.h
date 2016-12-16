#ifndef __COOLMONKEY_LUA_CONTEXT_H__
#define __COOLMONKEY_LUA_CONTEXT_H__

#include <string>

using std::string;

struct lua_State;

namespace CoolMonkey
{
    class LuaTableInstance;

    class LuaContext
    {
    public:
        void executeFile(string file_name);
        void executeString(string content);

        template<typename Ret, typename... Args>
        void addGlobalFunction(Ret(*fun_ptr)(Args...))
        {
            
        }

    public:
        LuaContext();
        ~LuaContext();

        lua_State* getLuaStateInstance() const;

    private:
        lua_State* m_lua_state{ nullptr };
    };
}

#endif