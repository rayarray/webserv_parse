#ifndef WS_CONFIGPARSER_HPP
# define WS_CONFIGPARSER_HPP

# include <string>
# include <fstream>
# include <vector>
# include "Server.hpp"

# define CONFIG_SETS = \
	"#DEFAULT CONFIG VALUES, DO NOT CHANGE OR MODIFY" \
	"HTTP 1" \
	"DEFAULTS 9" \
	"SERVER 9"

// usage: create instance with config file path, 
// start parsing with startParse which returns true if valid conf
// get servers with getServer, returns false when last server reached
// 
class ConfigParser {
	public:
		ConfigParser(const std::string &filepath);
		~ConfigParser();

		bool getConfigSets();
		bool startParse();
		bool getServer(Server &server);
		bool endParse();

		std::string _error;

	private:
		bool handleConfig();
		bool checkServer();
		bool skipWhiteSpace();
		bool skipWhiteSpaceLines();
		std::ifstream _cfile;
		std::string _line;
		size_t _pos;

		const std::vector<const std::string> top_level = 
			{"http", "server", "default"};
		const std::vector<const std::string> http_conf =
			{"foobar"};
		const std::vector<const std::string> config_sets;
};

#endif