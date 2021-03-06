#include <iostream>
#include "asio.hpp"
#include "script/lua/lua_context.h"
#include "script/lua/cfunction_lua_delegate.h"
#include "script/lua/luafunction_c_delegate.h"
#include "script/lua/luatype_c_delegate.h"

using UDP = asio::ip::udp;

class Server
{
public:
    Server(asio::io_context& io_context, short port)
        : m_socket(io_context, UDP::endpoint(UDP::v4(), port))
    {
        receive();
    }

    void receive()
    {
        m_socket.async_receive_from(
            asio::buffer(m_data, k_max_length),
            m_sender_endpoint,
            [this](std::error_code err, size_t received_byte_count)
            {
                if (!err && received_byte_count > 0)
                {
                    std::cout << "receive from  " << m_sender_endpoint.address().to_string() << ":" << m_sender_endpoint.port() << std::endl;
                    std::cout.write(m_data, received_byte_count);
                    std::cout << std::endl;
                    send(received_byte_count);
                }
                else
                {
                    std::cout << "receiver error: " << err.message() << std::endl;
                    receive();
                }
        });
    }

    void send(const size_t send_length)
    {
        m_socket.async_send_to(
            asio::buffer(m_data, send_length),
            m_sender_endpoint,
            [this](std::error_code err, size_t sended_byte_count)
            {
                if (err)
                {
                    std::cout << "send error: " << err.message() << std::endl;
                }

                receive();
            });
    }

private:
    static const size_t k_max_length = 1024;

private:
    UDP::socket m_socket;
    UDP::endpoint m_sender_endpoint;

    char m_data[k_max_length]{ 0 };
};

int testFunc(int a, float b, std::string c)
{
    std::cout << "int: " << a << std::endl;
    std::cout << "float: " << b << std::endl;
    std::cout << "string: " << c << std::endl;

    return a + b;
}

class TestVirtual
{
public:
    virtual int test2(int a, int b) = 0;

    int test(int a, int b, int c)
    {
        std::cout << "this is call base!" << std::endl;
        return a + b + c;
    }
};

class TestClass
    : public TestVirtual
{
public:
    int test2(int a, int b)
    {
        std::cout << "this is call drived!!!" << std::endl;
        return a * b;
    }

    int test(int a, int b)
    {
        std::cout << "this is call class member function!!!" << std::endl;

        return a + b;
    }
};

int main(int argc, char* argv[])
{
    CoolMonkey::LuaContext test_lua_context;
    test_lua_context.executeFile("d:/test_lua.lua");

    lua_getglobal(test_lua_context.getLuaStateInstance(), "wokao");
    CoolMonkey::LuaTableInstance table(test_lua_context.getLuaStateInstance(), 1);
    lua_pop(test_lua_context.getLuaStateInstance(), 1);

    CoolMonkey::LuaFunctionCDelegate test_lua_func(test_lua_context.getLuaStateInstance(), "add", table);
    CoolMonkey::LuaFunctionCDelegate test_lua_func_2(test_lua_context.getLuaStateInstance(), "sub");
    CoolMonkey::LuaFunctionCDelegate test_lua_func_3 = test_lua_func_2;

    std::cout << "lua return value: " << test_lua_func.invoke<int, int, int>(2, 5) << std::endl;
    std::cout << "lua return value2: " << test_lua_func_2.invoke<double, double, float>(2.4, 5.8) << std::endl;
    std::cout << "lua return value3: " << test_lua_func_3.invoke<double, double, float>(3.14, 9,8) << std::endl;

    test_lua_func_2 = test_lua_func;

    std::cout << "lua return value4: " << test_lua_func_2.invoke<int, int, int>(1, 2) << std::endl;

    CoolMonkey::pushCFunctionToLua(test_lua_context.getLuaStateInstance(), &testFunc, "testCall");
    TestClass a;
    TestVirtual* vitual_a = &a;
    //CoolMonkey::pushCFunctionToLua(test_lua_context.getLuaStateInstance(), &TestVirtual::test, "testClassCall");
    //CoolMonkey::pushCFunctionToLua(test_lua_context.getLuaStateInstance(), &TestVirtual::test2, "testClassCall2");
    //CoolMonkey::pushCFunctionToLua(test_lua_context.getLuaStateInstance(), &TestVirtual::test, "testClassCall3");
    //CoolMonkey::pushCFunctionToLua(test_lua_context.getLuaStateInstance(), &TestClass::test, "testClassCall4");

    test_lua_func_2 = test_lua_func_3;
    test_lua_func = test_lua_func_3;

    test_lua_context.executeFile("d:/test.lua");

    //std::cout << ClassHasMemberTypeClassType<CoolMonkey::TypeTraits::FunctionInfo<decltype(&TestClass::test)>>::value << std::endl;
    //std::cout << ClassHasMemberTypeClassType<CoolMonkey::TypeTraits::FunctionInfo<decltype(&testFunc)>>::value << std::endl;

    try
    {
        short port = 10000;
        if (argc >= 2)
        {
            port = std::atoi(argv[1]);
        }

        asio::io_context io_context;

        Server server(io_context, port);

        io_context.run();
    }
    catch(std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return 0;
}