#include <iostream>
#include <assert.h>

extern "C"
{
#include "uv.h"
}

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

uv_loop_t* loop = nullptr;

void onClose(uv_handle_t* handle);

void afterShutdown(uv_shutdown_t* req, int status)
{
    uv_close((uv_handle_t*)req->handle, onClose);
    delete req;
}

void allocateBuffer(
    uv_handle_t* handle,
    size_t suggested_size,
    uv_buf_t* buf)
{
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void afterWrite(uv_write_t* req, int status)
{
    write_req_t* write_request = reinterpret_cast<write_req_t*>(req);
    delete[] write_request->buf.base;
    delete write_request;

    if (status)
    {
        std::cout << "write error: " << uv_strerror(status) << std::endl;
    }

}

void afterRead(
    uv_stream_t* stream,
    ssize_t nread,
    const uv_buf_t* buf)
{
    if (nread < 0)
    {
        if (nread == UV_EOF)
        {
            std::cout << "read end of stream!" << std::endl;
        }
        else
        {
            std::cout << uv_strerror(nread) << std::endl;
        }

        delete[] buf->base;
        uv_shutdown_t* shutdown_request = new uv_shutdown_t;
        assert(uv_shutdown(shutdown_request, stream, afterShutdown) == 0);
        return;
    }

    if (nread == 0)
    {
        delete[] buf->base;
        return;
    }

    std::string received_content(buf->base, nread);
    std::cout << "receive: " << received_content.c_str() << std::endl;

    write_req_t* write_request = new write_req_t;
    write_request->buf = uv_buf_init(buf->base, nread);

    if (uv_write(&write_request->req, stream, &write_request->buf, 1, afterWrite))
    {
        std::cout << "write stream fail!" << std::endl;
        assert(false);
    }
}

void onClose(uv_handle_t* peer)
{
    delete peer;
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

    uv_tcp_t* client = new uv_tcp_t;
    uv_tcp_init(loop, client);

    if (uv_accept(server, (uv_stream_t*)client) == 0)
    {
        uv_read_start((uv_stream_t*)client, allocateBuffer, afterRead);
    }
    else
    {
        uv_close((uv_handle_t*)client, onClose);
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
    else
    {
        std::cout << "listening at: 10000 port" << std::endl;
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}