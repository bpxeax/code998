#include <iostream>
#include <string>
#include "asio.hpp"

using UDP = asio::ip::udp;

int main(int argc, char* argv[])
{
    try
    {
        std::string host("127.0.0.1");
        std::string port("10000");

        if (argc >= 3)
        {
            host = std::string(argv[1]);
            port = std::string(argv[2]);
        }

        asio::io_context looper;

        UDP::socket sender(looper, UDP::endpoint(UDP::v4(), 0));

        UDP::resolver resolver(looper);
        std::error_code resolve_err;
        UDP::endpoint end_point = resolver.resolve(host, port, resolve_err)->endpoint();

        if (resolve_err)
        {
            std::cerr << "resolve address error: " << resolve_err.message() << std::endl;
            return 1;
        }

        std::cout << "Please enter message: " << std::endl;
        char buffer[1024];
        std::cin.getline(buffer, 1024);
        size_t msg_length = std::strlen(buffer);
        sender.send_to(asio::buffer(buffer, msg_length), end_point);

        char reply[1024];
        UDP::endpoint replyer_endpoint;
        const size_t reply_length = sender.receive_from(asio::buffer(reply, 1024), replyer_endpoint);
        std::cout << "Reply: " << std::endl;
        std::cout.write(reply, reply_length);
        std::cout << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return 0;
}