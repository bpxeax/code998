#include <iostream>
#include "asio.hpp"
#include "script/lua/lua_context.h"
#include "script/lua/c_to_lua_function_delegate.h"

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

int main(int argc, char* argv[])
{
    CoolMonkey::LuaContext test_lua_context;

    CoolMonkey::CToLuaFunctionDelegate<int, int, float, std::string>::addFunction(test_lua_context.m_lua_state, testFunc, "testCall");

    test_lua_context.executeFile("d:/test.lua");

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