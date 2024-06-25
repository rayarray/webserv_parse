#include "Socket.hpp"

Socket::Socket(int port, ConfigServer &server) : _port(port), _server(server) {
    if (_fd = socket(AF_INET, SOCK_STREAM, 0))
        throw SocketException(strerror(errno));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));
    int const enable = 1;
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (bind(_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
        throw SocketException(strerror(errno));
    if (listen(_fd, 10) < 0)
        throw SocketException(strerror(errno));
}

struct sockaddr_in &Socket::getAddress() { return _address; }

int Socket::getFD() { return _fd; }
