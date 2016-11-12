#include <iostream>
#include <string>
#include "asio.hpp"

using UDP = asio::ip::udp;

class Client
{
public:
    Client(asio::io_context& looper, const UDP::endpoint& target_address)
        : m_sender_socket(looper, UDP::endpoint(UDP::v4(), 0))
        , m_target_address(target_address)
    {
        send();
    }

private:
    void send()
    {
        /*std::cout << "Please enter message: " << std::endl;
        std::cin.getline(m_buffer, 1024);
        size_t msg_length = std::strlen(m_buffer);*/

        static size_t k_count = 0;

        m_sender_socket.async_send_to(
            asio::buffer(&k_count, sizeof(k_count)),
            m_target_address,
            [this](std::error_code err, size_t sended_byte_count)
        {
            if (err)
            {
                std::cerr << "send error: " << err.message() << std::endl;
                send();
            }
            else
            {
                receive();
            }
        });

        k_count++;
    }

    void receive()
    {
        m_sender_socket.async_receive_from(
            asio::buffer(m_reply, 1024),
            m_replyer_endpoint,
            [this](std::error_code receive_err, size_t receive_byte_count)
        {
            if (!receive_err)
            {
                std::cout << "Reply from  " << m_replyer_endpoint.address().to_string() << ":" << m_replyer_endpoint.port() << std::endl;
                std::cout.write(m_reply, receive_byte_count);
                std::cout << std::endl;

                send();
            }
            else
            {
                std::cerr << "receive error: " << receive_err.message() << std::endl;
                receive();
            }
        });
    }

private:
    UDP::socket m_sender_socket;
    UDP::endpoint m_target_address;
    char m_buffer[1024];
    char m_reply[1024];
    UDP::endpoint m_replyer_endpoint;
};

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

        Client client(looper, end_point);

        looper.run();
    }
    catch(std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return 0;
}