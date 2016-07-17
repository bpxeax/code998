#include <iostream>

extern "C"
{
#include "uv.h"
}

void onConnect(uv_connect_t* req, int status)
{
    if (status)
    {
        std::cout << "connect error: " << uv_strerror(status) << std::endl;
        delete req;
        return;
    }

    std::cout << "connect success!!" << std::endl;
    delete req;
}

int main()
{
    uv_tcp_t* socket = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), socket);

    uv_connect_t* connect = new uv_connect_t;

    sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 10000, &dest);

    uv_tcp_connect(connect, socket, (const sockaddr*)&dest, onConnect);

    return 0;
}