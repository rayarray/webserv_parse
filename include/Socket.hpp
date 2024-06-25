#ifndef WS_SOCKET_HPP
# define WS_SOCKET_HPP
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include "ConfigServer.hpp"

class Socket {
    public:
        size_t const _port;

        Socket(int port, ConfigServer &server);

        struct sockaddr_in &getAddress();
        int getFD();
    private:
        int _fd;
        struct sockaddr_in _address;
        int _newSock;
        ConfigServer &_server;
    public:
        class SocketException : public std::exception {
			public:
    			SocketException(std::string const msg) : _msg(msg) {}
			    const char *what() const noexcept override { return _msg.c_str(); }
			private:
			    const std::string _msg;
		};
};

#endif