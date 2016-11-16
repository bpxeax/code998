#ifndef __COOLMONKEY_ASIO_WRAPPER_H__
#define __COOLMONKEY_ASIO_WRAPPER_H__

#include "asio.hpp"

namespace CoolMonkey
{
    using UDP = asio::ip::udp;
    using TCP = asio::ip::tcp;

    //using IOContext = asio::io_context;
    class IOContext : public asio::io_context{};
}

#endif
