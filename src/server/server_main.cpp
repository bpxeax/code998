#include <iostream>

extern "C"
{
#include "uv.h"
}

uv_loop_t* loop = nullptr;

void allocate_buffer(
    uv_handle_t* handle,
    size_t suggested_size,
    uv_buf_t* buf)
{

}

void echo_read(
    uv_stream_t* stream,
    ssize_t nread,
    const uv_buf_t* buf)
{

}

void onConnect(
    uv_stream_t* server, 
    int status)
{
    if (status < 0)
    {
        std::cout << "new connection fail with " << uv_strerror(status) << std::endl;
        return;
    }

    uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);

    if (uv_accept(server, (uv_stream_t*)client) == 0)
    {
        uv_read_start((uv_stream_t*)client, allocate_buffer, echo_read);
    }
    else
    {
        uv_close((uv_handle_t*)client, nullptr);
    }
}

int main()
{
    loop = uv_default_loop();

    uv_tcp_t server;

    uv_tcp_init(loop, &server);

    sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 10000, &addr);

    uv_tcp_bind(&server, (const sockaddr*)&addr, 0);

    int error_status = uv_listen((uv_stream_t*)&server, SOMAXCONN, onConnect);
    if (error_status)
    {
        std::cout << "listen failed with " << uv_strerror(error_status) << std::endl;
        return 1;
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}