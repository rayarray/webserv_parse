#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0), _server_index(0) {}

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
	//for (const std::string& line : _config_defaults) { // print _config_defaults
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

// bool ConfigParser::handleConfig() {
// 	std::cout << "handleC _pos[" << _pos << "] size[" << _line.size() << "]" << std::endl;
// 	if (_pos >= _line.size()) return false;
// 	size_t _end = _pos;
// 	while (_end < _line.size() && !ws_wspace(_line.at(_end))) {
// 		_end++;
// 	}
// 	return (std::cout << "handleConf: [" << _line.substr(_pos, _end - _pos) << "]" << std::endl, _pos = _line.size(), true);
// }

// ~ called by startParse and handleSubConfig, handles a config line and calls handleSubConfig in case of subsection
// * handles syntax at current line, advances _pos to end of line when done
bool ConfigParser::handleConfig() {
	size_t cfg_index;
	std::cout << "handleConfig: _line: " << _line.substr(_pos, std::string::npos) << std::endl;
	if (ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index)) {
		std::cout << "handleConfig match, index: " << cfg_index << ", word: " << _config_defaults.at(cfg_index) << std::endl;
		if (_config_defaults.at(cfg_index).back() == 'S' && !handleSubConfig(cfg_index))
			return false;
		else if (handleConfig(cfg_index))
			return false;
		_pos = _line.size();
		return true;
	}
	return false;
}

bool ConfigParser::handleConfig(size_t index) {
	if (!checkSyntax(index))
		return false;
	std::cout << "handleConfig(index): _line@_pos: " << _line.substr(_pos, std::string::npos) << std::endl;
	return true;
}

bool ConfigParser::handleSubConfig(size_t index) { (void)index;
	std::cout << "handleSubConfig called" << std::endl;
	return true;
}

// * checks format of syntax after keyword
// ! todo: add getConfig* funcs and rewrite this
bool ConfigParser::checkSyntax(const size_t index) {
	size_t store_pos = _pos;
	size_t wordsize = ws_getword(_config_defaults.at(index)).size();
	if (wordsize == 0 || wordsize == std::string::npos)
		return (_error = "Unexpected syntax in config defaults!", false);
	size_t def_pos = wordsize;
	size_t cfg_pos = wordsize + _pos;
	while (def_pos + 1 < _config_defaults.at(index).size()) { //&& cfg_pos + 1 < ws_size(_line)) {
		if (!ws_wspace(_line.at(cfg_pos)))
			return (_error = "Whitespace not found in expected position at keyword [" + _config_defaults.at(index).substr(0, wordsize) + "]", false);
		else if (!ws_wspace(_config_defaults.at(index).at(def_pos)))
			return (_error = "Expected whitespace not found in config defaults!", false);
		while (ws_wspace(_line.at(cfg_pos)))
			cfg_pos++;
		if (!checkSyntaxType(index, def_pos++, cfg_pos))
			return (_error = "Syntax error in config", false);
		while (def_pos + 1 < _config_defaults.at(index).size() && _config_defaults.at(index).at(def_pos - 1) == _config_defaults.at(index).at(def_pos)) {
			std::cout << "checkSyntax: repeated type" << std::endl;
			if (!checkSyntaxType(index, def_pos, cfg_pos))
				return (_error = "Syntax error in config", false);
			while (ws_wspace(_line.at(cfg_pos)))
				cfg_pos++;
			if (cfg_pos >= ws_size(_line))
				break;
		}
	}
	return (_pos = store_pos, true);
}

bool ConfigParser::checkSyntaxType(const size_t index, const size_t index_pos, size_t &cfg_index) { std::cout << "checkSyntaxType called, type: " << _config_defaults.at(index).at(index_pos + 1) << std::endl;
	const size_t cfg_start = cfg_index;
	bool quotes = false;
	switch (_config_defaults.at(index).at(index_pos)) {
		case 'N':
			while (!ws_endl(_line, cfg_index) && std::isdigit(_line.at(cfg_index)))
				cfg_index++;
			if (cfg_index - cfg_start > 10)
				return (_error = "Too many digits in numeric value", false);
			if (!ws_endl(_line, cfg_index) && (_line.at(cfg_index) == 'k' || _line.at(cfg_index) == 'M'))
				cfg_index++;
			if (ws_endl(_line, cfg_index) || ws_wspace(_line.at(cfg_index)))
				return true;
			break;
		case 'T':
			while ((!ws_endl(_line, cfg_index) || (quotes && cfg_index < _line.size())) && 
					(!ws_wspace(_line.at(cfg_index) || quotes) && std::isprint(_line.at(cfg_index)))) {
				if (_line.at(cfg_index) == '"')
					quotes = (!quotes) ? true : false;
				cfg_index++;
				if (!quotes && (ws_endl(_line, cfg_index) || ws_wspace(_line.at(cfg_index))))
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