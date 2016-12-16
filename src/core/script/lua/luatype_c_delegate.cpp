#include "luatype_c_delegate.h"
#include "lua_context.h"
#include <iostream>

namespace CoolMonkey
{
    LuaTableInstance::LuaTableInstance()
        : m_lua_state(nullptr)
        , m_table_ref(LUA_NOREF)
    {
        m_ref_count = new unsigned int;
        *m_ref_count = 1;
    }

    LuaTableInstance::LuaTableInstance(lua_State* lua_state, const int value_index)
        : m_lua_state(lua_state)
    {
        if (lua_istable(m_lua_state, -value_index))
        {
            lua_pushvalue(m_lua_state, -value_index);
            m_table_ref = lua_ref(m_lua_state, 1);
        }
        else
        {
            m_table_ref = LUA_NOREF;
        }

        m_ref_count = new unsigned int;
        *m_ref_count = 1;
    }

    LuaTableInstance::LuaTableInstance(const LuaTableInstance& rhs_table_instance)
    {
        ++(*rhs_table_instance.m_ref_count);

        m_lua_state = rhs_table_instance.m_lua_state;
        m_table_ref = rhs_table_instance.m_table_ref;
        m_ref_count = rhs_table_instance.m_ref_count;
    }

    LuaTableInstance& LuaTableInstance::operator=(const LuaTableInstance& rhs_table_instance)
    {
        ++(*rhs_table_instance.m_ref_count);

        --(*m_ref_count);
        if ((*m_ref_count) == 0)
        {
            lua_unref(m_lua_state, m_table_ref);
            delete m_ref_count;

            std::cout << "hehe nimahai copy" << std::endl;
        }

        m_lua_state = rhs_table_instance.m_lua_state; 
        m_table_ref = rhs_table_instance.m_table_ref;
        m_ref_count = rhs_table_instance.m_ref_count;

        return (*this);
    }

    LuaTableInstance::LuaTableInstance(LuaTableInstance&& rhs_table_instance) noexcept
    {
        m_lua_state = rhs_table_instance.m_lua_state;
        m_table_ref = rhs_table_instance.m_table_ref;
        m_ref_count = rhs_table_instance.m_ref_count;

        rhs_table_instance.m_lua_state = nullptr;
        rhs_table_instance.m_table_ref = LUA_NOREF;
        rhs_table_instance.m_ref_count = nullptr;
    }

    LuaTableInstance& LuaTableInstance::operator=(LuaTableInstance&& rhs_table_instance) noexcept
    {
        // ensure self move to self to be right
        ++(*rhs_table_instance.m_ref_count);

        --(*m_ref_count);
        if ((*m_ref_count) == 0)
        {
            lua_unref(m_lua_state, m_table_ref);
            delete m_ref_count;

            std::cout << "hehe nimahai move" << std::endl;
        }

        --(*rhs_table_instance.m_ref_count);

        m_lua_state = rhs_table_instance.m_lua_state;
        m_table_ref = rhs_table_instance.m_table_ref;
        m_ref_count = rhs_table_instance.m_ref_count;

        rhs_table_instance.m_lua_state = nullptr;
        rhs_table_instance.m_table_ref = LUA_NOREF;
        rhs_table_instance.m_ref_count = nullptr;

        return (*this);
    }

    LuaTableInstance::~LuaTableInstance()
    {
        if (m_ref_count)
        {
            --(*m_ref_count);
            if (*m_ref_count == 0)
            {
                lua_unref(m_lua_state, m_table_ref);
                delete m_ref_count;

                std::cout << "hehe nimahai" << std::endl;
            }
        }

        m_lua_state = nullptr;
        m_table_ref = LUA_NOREF;
        m_ref_count = nullptr;
    }

    int LuaTableInstance::getValueToStack() const
    {
        if (m_lua_state)
        {
            lua_getref(m_lua_state, m_table_ref);
            return 1;
        }

        return 0;
    }
}
