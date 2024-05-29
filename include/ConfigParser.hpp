#ifndef WS_CONFIGPARSER_HPP
# define WS_CONFIGPARSER_HPP

# include <string>
# include <fstream>
# include <vector>
# include "Server.hpp"

// usage: create instance with config file path, 
// start parsing with startParse() which returns true if valid conf
// get servers with getServer(), returns false when last server reached
// finish parse with endParse(), returns true if parsing finished successfully
class ConfigParser {
	public:
		ConfigParser(const std::string &filepath);
		~ConfigParser();

		bool startParse();
		bool getServer(Server &server);
		bool endParse();

		std::string _error;

	private:
		bool getConfigSets();
		bool handleConfig();
		bool checkServer();
		bool checkSyntax();
		bool skipWhiteSpace();
		bool skipWhiteSpaceLines();
		std::ifstream _cfile;
		std::string _line;
		size_t _pos;

		std::vector<std::string> config_defaults;
};

#endif