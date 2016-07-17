#include <iostream>
#include <string>
#include <assert.h>

extern "C"
{
#include "uv.h"
}

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void onClose(uv_handle_t* peer)
{
    delete peer;
}

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

    if (status)
    {
        std::cout << "write error: " << uv_strerror(status) << std::endl;

        delete[] write_request->buf.base;
        delete write_request;

        uv_shutdown_t* shutdown_req = new uv_shutdown_t();
        uv_shutdown(shutdown_req, req->handle, afterShutdown);
        return;
    }

    std::cout << "you: ";
    std::string content;
    std::cin >> content;

    memcpy(write_request->buf.base, content.c_str(), write_request->buf.len);

    uv_write(&write_request->req, req->handle, &write_request->buf, 1, afterWrite);
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
            std::cout << "read error: " << uv_strerror(nread) << std::endl;
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

    std::string receive_content(buf->base, nread);
    std::cout << "server: " << receive_content.c_str() << std::endl;
    delete[] buf->base;
}

void onConnect(uv_connect_t* connection, int status)
{
    if (status)
    {
        std::cout << "connect error: " << uv_strerror(status) << std::endl;
        delete connection;
        return;
    }

    std::cout << "you: ";
    std::string content;
    std::cin >> content;

    uv_buf_t buffer;
    buffer.len = content.length();
    buffer.base = new char[buffer.len];
    memcpy(buffer.base, content.c_str(), buffer.len);

    write_req_t* write_request = new write_req_t();
    write_request->buf = buffer;

    int operator_status = 0;
    if (operator_status = uv_write(&write_request->req, connection->handle, &write_request->buf, 1, afterWrite))
    {
        std::cout << "start write error: " << uv_strerror(operator_status) << std::endl;

        uv_shutdown_t* shutdown_req = new uv_shutdown_t();
        uv_shutdown(shutdown_req, connection->handle, afterShutdown);
        return;
    }
    
    if (operator_status = uv_read_start(connection->handle, allocateBuffer, afterRead))
    {
        std::cout << "start read error: " << uv_strerror(operator_status) << std::endl;

        uv_shutdown_t* shutdown_req = new uv_shutdown_t();
        uv_shutdown(shutdown_req, connection->handle, afterShutdown);
        return;
    }
}

int main()
{
    uv_tcp_t* socket = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), socket);

    uv_connect_t* connect = new uv_connect_t;

    sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 10000, &dest);

    uv_tcp_connect(connect, socket, (const sockaddr*)&dest, onConnect);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}