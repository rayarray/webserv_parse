#ifndef WS_CONFIGREFERENCE_HPP
# define WS_CONFIGREFERENCE_HPP

# include <string>
# include <fstream>
# include <vector>
# include <iostream> // ! debug
# include "ConfigParser.hpp" // for GLOBAL definition
# include "ws_functions.hpp" // ? might be unnecessary

class ConfigReference {
	public:
		ConfigReference(const std::string &reference_file);
		~ConfigReference();
	
		bool keyExists(const std::string keyword);
		bool keyExists(const std::string section, const std::string keyword);
		size_t keyNumParam(const std::string keyword);
		char keyParamType(const std::string keyword, const size_t param_num);
		bool keyParamTypeMatch(const std::string keyword, const size_t param_num, const char type);

		void print(); // ! debug

	private:
		bool validType(const char type);
		bool processLine(const std::string &line);
		std::vector<std::vector<std::string> > _references;
		std::string _section;
};

#endif