#ifndef SEEERVER_TCPPROXY_H
#define SEEERVER_TCPPROXY_H

#include <boost/asio.hpp>

using namespace boost::asio;

class TcpProxy {

public:

    TcpProxy(io_service & io_service, std::string & local_host_address, unsigned short local_port_number,
             std::string & remote_address, unsigned short remote_port);
    bool acceptConnections();


private:
    io_service & the_io_service;
    ip::address the_address;
    unsigned short local_port;
    std::string remote_address;
    unsigned short remote_port;
    ip::tcp::acceptor the_acceptor;
    std::shared_ptr<class Connections> connections_ptr;
    void handleAccept(const boost::system::error_code & error);

};

#endif //SEEERVER_TCPPROXY_H
