#ifndef WS_CONFIGPARSER_HPP
# define WS_CONFIGPARSER_HPP

#include <cstring>
#include <fstream>
#include "Server.hpp"

// usage: create instance with config file path, 
// start parsing with startParse which returns true if valid conf
// get servers with getServer, returns false when last server reached
// 
class ConfigParser {
	public:
		ConfigParser(const std::string &filepath);
		~ConfigParser();

		bool startParse();
		bool getServer(Server &server);

		std::string _error;

	private:
		std::ifstream _cfile;
		std::string _line;
		int	_pos;
};

#endif