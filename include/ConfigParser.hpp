#ifndef WS_CONFIGPARSER_HPP
# define WS_CONFIGPARSER_HPP

# include <string>
# include <fstream>
# include <vector>
# include "Server.hpp"

# define	GLOBAL "global" // config section name for top level

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

		void printCS();

		std::string _error;

	private:
		bool getConfigSets();
		//const std::string& getConfig(size_t index);
		//const size_t getConfigArgNum(size_t index);
		//const char& getConfigArg(size_t index);
		bool handleConfig();
		bool handleConfig(size_t index);
		bool handleSubConfig(size_t index);
		bool checkSyntax(const size_t index);
		bool checkSyntax(size_t index, bool repeat);
		bool checkSyntaxType(const char syntax_type, size_t &cfg_index);
		bool checkServer(size_t index);
		bool skipWhiteSpace();
		bool skipWhiteSpaceLines();

		std::ifstream _cfile;
		std::string _line;
		size_t _pos;
		size_t _server_index;
		std::vector<std::string> _config_defaults;
		std::string _section;
		std::vector<ConfigSection> _config_sections;
};

#endif