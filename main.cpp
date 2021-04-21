#include <boost/asio.hpp>

#include <locale>
#include "TcpProxy.h"


int main() {

    boost::asio::io_service the_io_service;
    std::string localhost_add = "127.0.0.1";
    unsigned short local_port = 8866;

    unsigned short server_port = 8867;

    TcpProxy server(the_io_service, localhost_add, local_port, localhost_add, server_port);
    server.acceptConnections();

    the_io_service.run();

}
