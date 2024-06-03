#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0), _server_index(0), _section (GLOBAL) {}

ConfigParser::~ConfigParser() {
	if (_cfile) _cfile.close();
}

bool ConfigParser::getConfigSets() {
	std::ifstream config_file(CONFIG_DEFAULTS);
	if (!config_file) return (_error = "Error opening config values file", false);
	if (!std::getline(config_file, _line) || _line != "# INTERNAL FILE, DO NOT MODIFY") 
		return (_error = "Invalid config defaults file", false);
	while (std::getline(config_file, _line))
		_config_defaults.push_back(_line);
	//for (const std::string& line : _config_defaults) { // * print _config_defaults
	//	std::cout << line << std::endl;
	//}
	if (!ws_checkword("SERVER", _config_defaults, _server_index))
		return (_error = "Invalid config defaults file", false);
	return true;
}

bool ConfigParser::startParse() {
	if (!_cfile) return (_error = "Error opening config file", false);
	if (!getConfigSets()) return (false);
	std::getline(_cfile, _line);
	while (!skipWhiteSpaceLines()) {
		std::cout << "startParse calling handleConfig with: " << _line <<  std::endl;
		if (checkServer(_pos))
			return true;
		if (!handleConfig())
			return (_error = "Parse error: " + _error, false);
	}
	_error = _line;
	std::cout << "startParse successful" << std::endl;
	return true;
}


bool ConfigParser::getServer(Server &server) { (void)server;
	return true;
}

bool ConfigParser::endParse() {
	if (_cfile) _cfile.close();
	return true;
}

// ~ called by startParse and handleSubConfig, handles a config line and calls handleSubConfig in case of subsection
// * handles syntax at current line, advances _pos to end of line when done
bool ConfigParser::handleConfig() {
	size_t cfg_index;
	std::cout << "handleConfig: _line: " << _line.substr(_pos, std::string::npos) << std::endl;
	if (ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index)) {
		std::cout << "handleConfig match, index: " << cfg_index << ", word: " << _config_defaults.at(cfg_index) << std::endl;
		if (_config_defaults.at(cfg_index).back() == 'S' && !handleSubConfig(cfg_index))
			return (_error = "Subsection parsing failed at section: " + _section + " error: " + _error, false);
		else if (!handleConfig(cfg_index))
			return (_error = "Line parsing failed: " + _error, false);
		_pos = _line.size();
		return true;
	}
	return false;
}

bool ConfigParser::handleConfig(size_t index) {
	if (!checkSyntax(index))
		return (_error = "Syntax error: " + _error, false);
	std::cout << "handleConfig(index): _line@_pos: " << _line.substr(_pos, std::string::npos) << std::endl;
	return true;
}

bool ConfigParser::handleSubConfig(size_t index) { std::cout << "handleSubConfig called" << std::endl;
	if (_section != GLOBAL) return false;
	ConfigSection *section = nullptr;
	for (size_t i = 0; i < _config_sections.size(); i++) {
		if (section == nullptr && ws_getword(_config_defaults.at(index)) == _config_sections.at(i)._section_name)
			section = &_config_sections.at(i);
	}
	if (section == nullptr) {
		_config_sections.push_back(ConfigSection(ws_getword(_config_defaults.at(index))));
		section = &_config_sections.back();
		std::vector<std::string> initvalues;
		initvalues.push_back(ws_getword(_config_defaults.at(index)));
		for (size_t i = 1; i < ws_getarglen(_config_defaults.at(index)); i++) {
			
		}
		//section->addConfigLine();
	}
	_section = ws_getword(_config_defaults.at(index));
	return true;
}

// ~ called by handleConfig, does not modify _pos
// checks syntax for _line against _config_defaults at index
// * seems to work, did some prelim testing
bool ConfigParser::checkSyntax(const size_t index) { std::cout << "checkSyntax called" << std::endl;
	size_t wordsize = ws_getword(_config_defaults.at(index)).size();
	if (wordsize == 0 || wordsize == std::string::npos)
		return (_error = "Unexpected syntax in config defaults!", false);
	size_t cfg_pos = wordsize + _pos;
	for (size_t i = 1; ws_getarg(i, _config_defaults.at(index)) != 0; i++) {
		std::cout << "checkSyntax [" << ws_getarg(i, _config_defaults.at(index)) << "][" << _line.substr(cfg_pos, -1) << "]" << std::endl;
		std::cout << "checkSyntax getarglen: " << ws_getarglen(_config_defaults.at(index)) << std::endl;
		while (!ws_endl(_line, cfg_pos) && ws_wspace(_line.at(cfg_pos)))
			cfg_pos++;
		if (ws_endl(_line, cfg_pos) && i <= ws_getarglen(_config_defaults.at(index)))
			return (_error = "Unexpected end of line", false);
		if (ws_endl(_line, cfg_pos) && i > ws_getarglen(_config_defaults.at(index)))
			return true;
		if (!checkSyntaxType(ws_getarg(i, _config_defaults.at(index)), cfg_pos))
			return (_error = "Syntax error: " + _error, false);
		if (ws_getarg(i + 1, _config_defaults.at(index)) == 0 && i == ws_getarglen(_config_defaults.at(index)))
			return true;
	}
	return (_error = "Zero or too many arguments at keyword: " + ws_getword(_config_defaults.at(index)), false);
}

// cfg_index is position at _line
bool ConfigParser::checkSyntaxType(const char syntax_type, size_t &cfg_index) { std::cout << "checkSyntaxType called, type: " << syntax_type << std::endl;
	std::cout << "cST substr: [" << _line.substr(cfg_index, -1) << "]" << std::endl;
	const size_t cfg_start = cfg_index;
	bool quotes = false;
	switch (syntax_type) {
		case 'N':
			while (!ws_endl(_line, cfg_index) && std::isdigit(_line.at(cfg_index)))
				cfg_index++;
			if (cfg_index - cfg_start > 10)
				return (_error = "Too many digits in numeric value", false);
			if (!ws_endl(_line, cfg_index) && (_line.at(cfg_index) == 'k' || _line.at(cfg_index) == 'M'))
				cfg_index++;
			if (cfg_index - cfg_start > 0 && (ws_endl(_line, cfg_index) || ws_wspace(_line.at(cfg_index))))
				return true;
			break;
		case 'T':
			while ((!ws_endl(_line, cfg_index) || (quotes && cfg_index < _line.size())) && 
					(!ws_wspace(_line.at(cfg_index) || quotes) && std::isprint(_line.at(cfg_index)))) {
				if (_line.at(cfg_index) == '"')
					quotes = (!quotes) ? true : false;
				cfg_index++;
				if (cfg_index - cfg_start > 0 && !quotes && ((ws_endl(_line, cfg_index) || ws_wspace(_line.at(cfg_index)))))
					return true;
			}
			break;
		case 'S':
			if (!ws_endl(_line, cfg_index) && _line.at(cfg_index) == '{' && ++cfg_index) {
				while (!ws_endl(_line, cfg_index) || ws_wspace(_line.at(cfg_index)))
					cfg_index++;
				if (ws_endl(_line, cfg_index)) 
					return true;
			}
			break;
		default:
			break;
	}
	return (_error = "Unknown syntax in config defaults", false);
}

bool ConfigParser::checkServer(size_t index) {
	if (_line.find("server") == index && index + 7 < _line.size() && ws_wspace(_line.at(index + 6))) {
		index += 6;
		while (!ws_endl(_line, index) && ws_wspace(_line.at(index)))
			index++;
		if (ws_endl(_line, index) || _line.at(index) != '{')
			return (_error = "Invalid syntax at server section begin at character ' '", _error[53] = _line.at(index),  false);		
		while (index++ && !ws_endl(_line, index) && ws_wspace(_line.at(index)))
			;		
		if (ws_endl(_line, index))
			return true;
	}
	//return (_error = "Extra non-whitespace characters present in server section start", false);
	return false;
}

// skips white space characters and jumps to end on comment character(#) on stored _line
// returns true if no parseable content left, else false
bool ConfigParser::skipWhiteSpace() {
	while (_pos < _line.size() && ws_wspace(_line.at(_pos)))
		_pos++;
	if (_pos >= _line.size() || _line.at(_pos) == '#')
		return (_pos = _line.size(), true);
	return false;
}

// skips whitespaces and comments on line or multiple lines
// returns true if reached end of config file
bool ConfigParser::skipWhiteSpaceLines() {
	while (skipWhiteSpace()) {
		_pos = 0;
		if (!std::getline(_cfile, _line))
			return true;
	}
	return false;
}