#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _server_index(0), _ref("config_defaults") ,_cfg(filepath) {
	_config_sections.push_back(ConfigSection(GLOBAL));
}

ConfigParser::~ConfigParser() {}

bool ConfigParser::startParse() {
	while (_cfg.nextLine()) {
		_cfg.processLine();
		storeConfigLine();
		_cfg.print();
		if (_cfg.getSection() == "server")
			break;
		
	}
	if (_cfg.getSection() != "server")
		throw std::runtime_error("Configuration file is missing a server section!");
	return (true);
}

Server ConfigParser::getServer() {
	if (_cfg.getSection() != "server" || _cfg.getLastWord() != "{")
		throw std::runtime_error("Internal error: ConfigParser::getServer executed on non-server begin line");
	if (!_cfg.nextLine() || !_cfg.processLine() || _cfg.getSection() != "server" || _cfg.getWord(0) != "listen")
		throw std::runtime_error("The first line in server has to be listen!");
	size_t port = 80;
	if (_cfg.getWord(1).find(":") != std::string::npos && _cfg.getWord(1).find(":") != 0 && _cfg.getWord(1).find(":") != _cfg.getWord(1).size() - 1) {
		try { port = std::stol(_cfg.getWord(1).substr(_cfg.getWord(1).find(":") + 1, -1)); }
		catch (std::exception &e) { 
			std::string error(e.what());
			error = "Caught exception trying to convert a value: " + error;
			throw std::runtime_error(error); 
		}
	}
	//std::cout << "creating server with " << _cfg.getWord(1).substr(0, _cfg.getWord(1).find(":")) << " with port " << port << std::endl;
	Server srv(_cfg.getWord(1).substr(0, _cfg.getWord(1).find(":")), port);
	return srv;
}

bool ConfigParser::endParse() {
	return true;
}

bool ConfigParser::storeConfigLine() {
	if (_cfg.getWord(0) == "}")
		return true;
	for (ConfigSection &section : _config_sections) {
		if (section._section_name == _cfg.getSection()) {
			for (const std::string &s : _cfg.getVector())
				std::cout << s << "-";
			std::cout << std::endl;
			section.addConfigLine(_cfg.getVector());
			return true;
		}
	}
	ConfigSection section(_cfg.getSection());
	section.addConfigLine(_cfg.getVector());
	for (const std::string &s : _cfg.getVector())
				std::cout << s << "-";
			std::cout << std::endl;
	_config_sections.push_back(section);
	return true;
}

void ConfigParser::printCS() {
	for (size_t i = 0; i < _config_sections.size(); i++)
		_config_sections.at(i).printAll();
	// for (const ConfigSection &section : _config_sections)
	// 	for (const std::string &s : section)

	std::cout << "END" << std::endl;
}