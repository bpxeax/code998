#include "connection.h"
#include "asio_wrapper.h"

namespace CoolMonkey
{
    class TCPConnection
        : public Connection
    {
    public:
        TCPConnection(IOContext* io_context, TCP::endpoint end_point)
        {
            
        }

    private:
        IOContext* m_io_context{ nullptr };
        TCP::endpoint m_target_endpoint;
    };

    class UDPConnection
        : public Connection
    {
        
    };

    Connection* ConnectionFactory::newConnection(
        const ConnectionType type, 
        const std::string host, 
        const std::string port)
    {
        return nullptr;
    }

    void ConnectionFactory::deleteConnection(
        Connection * connection)
    {

    }
}
