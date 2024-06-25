#ifndef WS_MANAGER_HPP
# define WS_MANAGER_HPP
# include <vector>
# include "Socket.hpp"
# include "ConfigParser.hpp"

# define DEFAULTCONFIG  "cfg/default.conf"
# define RESPONSETEXTS  "cfg/default_responses"

class Manager {
    public:
        Manager(std::string config_file = DEFAULTCONFIG);

    private:
        std::vector<ConfigServer> _servers;
        ConfigSection _responses;
        std::vector<Socket> _sockets;
};

#endif