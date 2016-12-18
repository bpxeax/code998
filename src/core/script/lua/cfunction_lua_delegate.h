#ifndef __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__
#define __COOLMONKEY_CFUNCTION_LUA_DELEGATE_H__

#include <sstream>
#include "type_traits/type_traits_extensions.h"
#include "type_traits/function_traits.h"
#include "ctype_lua_delegate.h"
#include "lua_common_operations.h"
#include "lua.hpp"

using std::ostringstream;
using std::get;

namespace CoolMonkey
{
    GENERATE_CLASS_HAS_MEMBER_TYPE(ClassType)

    template<typename TFunc>
    class CToLuaFunctionDelegate
    {
    public:
        static int DelegateFunction(lua_State* lua_state)
        {
            int in_param_num = lua_gettop(lua_state);

            if (in_param_num != TypeTraits::FunctionInfo<TFunc>::arg_count)
            {
                ostringstream err_stream;
                err_stream << "lua call param count not match c funciton! " << "in: " << in_param_num << " desired: " << TypeTraits::FunctionInfo<TFunc>::arg_count << std::endl;
                lua_pushstring(lua_state, err_stream.str().c_str());
                lua_error(lua_state);

                return 0;
            }

            return callFunction(lua_state, typename TypeTraits::SequenceGen<TypeTraits::FunctionInfo<TFunc>::arg_count>::type());
        }

    private:
        template<size_t... seq, typename Func = TFunc>
        static int callFunction(
            lua_State* lua_state, 
            TypeTraits::IndexSequence<seq...>, 
            typename std::enable_if<!ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<Func>>::value && !std::is_same<typename TypeTraits::FunctionInfo<TFunc>::ResultType, void>::value>::type* = nullptr)
        {
            TFunc func_ptr = static_cast<TFunc>(TypeTraits::FunctionPointer<TFunc>(lua_touserdata(lua_state, lua_upvalueindex(1))));

            auto result = func_ptr(CToLuaTypeDelegate<typename TypeTraits::FunctionInfo<TFunc>::template ArgType<seq>::type>::getValueFromLua(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count - seq))...);
            lua_pop(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count));

            pushValuesToLua(lua_state, result);

            return 1;
        }

        template<size_t... seq, typename Func = TFunc>
        static int callFunction(
            lua_State* lua_state,
            TypeTraits::IndexSequence<seq...>,
            typename std::enable_if<!ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<Func>>::value && std::is_same<typename TypeTraits::FunctionInfo<TFunc>::ResultType, void>::value>::type* = nullptr)
        {
            TFunc func_ptr = static_cast<TFunc>(TypeTraits::FunctionPointer<TFunc>(lua_touserdata(lua_state, lua_upvalueindex(1))));

            func_ptr(CToLuaTypeDelegate<typename TypeTraits::FunctionInfo<TFunc>::template ArgType<seq>::type>::getValueFromLua(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count - seq))...);
            lua_pop(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count));

            return 0;
        }

        template<size_t... seq, typename Func = TFunc>
        static int callFunction(
            lua_State* lua_state, 
            TypeTraits::IndexSequence<seq...>, 
            typename std::enable_if<ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<Func>>::value && !std::is_same<typename TypeTraits::FunctionInfo<TFunc>::ResultType, void>::value>::type* = nullptr)
        {
            typedef typename TypeTraits::FunctionInfo<TFunc>::ClassType ClassType;

            TFunc func_ptr = static_cast<TFunc>(TypeTraits::FunctionPointer<TFunc>(lua_touserdata(lua_state, lua_upvalueindex(1))));
            ClassType* class_instance = static_cast<ClassType*>(lua_touserdata(lua_state, lua_upvalueindex(2)));

            auto result = (class_instance->*func_ptr)(CToLuaTypeDelegate<typename TypeTraits::FunctionInfo<TFunc>::template ArgType<seq>::type>::getValueFromLua(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count - seq))...);
            lua_pop(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count));

            pushValuesToLua(lua_state, result);

            return 1;
        }

        template<size_t... seq, typename Func = TFunc>
        static int callFunction(
            lua_State* lua_state,
            TypeTraits::IndexSequence<seq...>,
            typename std::enable_if<ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<Func>>::value && std::is_same<typename TypeTraits::FunctionInfo<TFunc>::ResultType, void>::value>::type* = nullptr)
        {
            typedef typename TypeTraits::FunctionInfo<TFunc>::ClassType ClassType;

            TFunc func_ptr = static_cast<TFunc>(TypeTraits::FunctionPointer<TFunc>(lua_touserdata(lua_state, lua_upvalueindex(1))));
            ClassType* class_instance = static_cast<ClassType*>(lua_touserdata(lua_state, lua_upvalueindex(2)));

            (class_instance->*func_ptr)(CToLuaTypeDelegate<typename TypeTraits::FunctionInfo<TFunc>::template ArgType<seq>::type>::getValueFromLua(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count - seq))...);
            lua_pop(lua_state, static_cast<int>(TypeTraits::FunctionInfo<TFunc>::arg_count));

            return 0;
        }
    };

    template<typename TFunc>
    typename std::enable_if<!ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<TFunc>>::value>::type 
    pushCFunctionToLua(lua_State* lua_state, TFunc func, const string& func_name)
    {
        lua_pushlightuserdata(lua_state, static_cast<void*>(TypeTraits::FunctionPointer<TFunc>(func)));
        lua_pushcclosure(lua_state, CToLuaFunctionDelegate<TFunc>::DelegateFunction, 1);
        lua_setglobal(lua_state, func_name.c_str());
    }

    template<typename TFunc>
    typename std::enable_if<ClassHasMemberTypeClassType<TypeTraits::FunctionInfo<TFunc>>::value>::type 
    pushCFunctionToLua(lua_State* lua_state, TFunc func, const string& func_name, typename TypeTraits::FunctionInfo<TFunc>::ClassType* class_instance)
    {
        lua_pushlightuserdata(lua_state, static_cast<void*>(TypeTraits::FunctionPointer<TFunc>(func)));
        lua_pushlightuserdata(lua_state, static_cast<void*>(class_instance));
        lua_pushcclosure(lua_state, CToLuaFunctionDelegate<TFunc>::DelegateFunction, 2);
        lua_setglobal(lua_state, func_name.c_str());
    }
}

#endif
