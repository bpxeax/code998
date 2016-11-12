#ifndef __COOLMONKEY_CONNECTION_H__
#define __COOLMONKEY_CONNECTION_H__

#include <string>
#include <assert.h>

namespace CoolMonkey
{
    class Connection
    {
        /// Peer to peer
        /// transfer message, keep alive
    public:
        virtual bool sendData(const void* buffer, const size_t buffer_count) = 0;
    };

    enum class ConnectionType : char
    {
        _TCP,
        _UDP
    };

    class IOContext;
    class ConnectionFactory
    {
    public:
        ConnectionFactory(IOContext* io_context)
            : m_io_context(io_context)
        {
            assert(io_context);
        }

        Connection* newConnection(const ConnectionType type, const std::string host, const std::string port);
        void deleteConnection(Connection* connection);

    private:
        IOContext* m_io_context{ nullptr };
    };
}

#endif
