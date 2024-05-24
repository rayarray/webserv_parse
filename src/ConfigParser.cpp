#include "ConfigParser.hpp"

// usage: create instance with config file path, 
// start parsing with startParse which returns true if valid conf
// get servers with getServer, returns false when last server reached


ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0) {}

ConfigParser::~ConfigParser() {
	if (_cfile)
		_cfile.close();
}

bool ConfigParser::startParse() {
	if (!_cfile) return (_error = "Error opening config file", false);
	std::getline(_cfile, _line);
	
}

bool ConfigParser::getServer(Server &server) {

}