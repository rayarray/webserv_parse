#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _server_index(0), _ref(filepath) ,_cfg("config_defaults") {
	_config_sections.push_back(ConfigSection(GLOBAL));
}

ConfigParser::~ConfigParser() {}

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
	return true;
}

// ~ called by startParse and handleSubConfig, handles a config line and calls handleSubConfig in case of subsection
// * handles syntax at current line, advances _pos to end of line when done
bool ConfigParser::handleConfig() {
	size_t cfg_index;
	size_t section_index = 0;
	std::cout << "hC _line.substr(_pos, -1): " << _line.substr(_pos, std::string::npos) << std::endl;
	if (_section != GLOBAL && !ws_checkword(ws_toupper(_section), _config_defaults, section_index) && ++section_index)
		return (_error = "Subsection keywords not found in config defaults", false);
	std::cout << "hC: _section: " << _section << " section_index: " << section_index << std::endl;
	std::cout << "hC: _line: " << _line.substr(_pos, std::string::npos) << std::endl;
	if (_section != GLOBAL) 
		std::cout << "hC: ws_checkword_lower: " << std::boolalpha << ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index, section_index) << std::endl;
	if ((_section == GLOBAL && ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index)) || (
		_section != GLOBAL && ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index, section_index))) {
		std::cout << "handleConfig match, index: " << cfg_index << ", word: " << _config_defaults.at(cfg_index) << std::endl;
		if (_config_defaults.at(cfg_index).back() == 'S') {
			if (!handleSubConfig(cfg_index))
				return (_error = "Subsection parsing failed at section: " + _section + ", error: " + _error, false);
		}
		else if (!handleConfig(cfg_index))
			return (_error = "Line parsing failed: " + _error, false);
		_pos = _line.size();
		return true;
	}
	return (_error = "Keyword not found, cfg_index: " + std::to_string(cfg_index), false); // ! index not found, fix pls
}

bool ConfigParser::handleConfig(size_t index) {
	if (_section != GLOBAL) 
		index++;
	if (!checkSyntax(index))
		return (std::cout << "hC failed at checkSyntax" << std::endl, _error = "Syntax error: " + _error, false);
	std::cout << "handleConfig(index): _line@_pos: " << _line.substr(_pos, std::string::npos) << std::endl;
	for (ConfigSection &section : _config_sections) {
		if (section._section_name == _section) {
			std::vector<std::string> values;
			for (size_t i = 1; i <= ws_getarglen(_config_defaults.at(index)); i++)
				values.push_back(ws_getargstr(i, _line));
			section.addConfigLine(values);
	}	}
	_pos = _line.size();
	return (std::cout << "hC returning true" << std::endl, true);
}

bool ConfigParser::handleSubConfig(size_t index) { std::cout << "handleSubConfig called" << std::endl;
	std::string topsection = _section;
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
			initvalues.push_back(ws_getargstr(i, _line));
			//std::cout << "subconfig " << ws_getword(_config_defaults.at(index)) << " initvalue: " << ws_getargstr(i, _line) << std::endl;
		}
		section->addConfigLine(initvalues);
	}
	_section = ws_getword(_config_defaults.at(index));
	//if (_pos = 0, !std::getline(_cfile, _line))
	//	return (_error = "Unexpected end of file at section " + _section + " start", false);
	std::cout << "hSC _line at start of handling lines for section : " << _line << " pos: " << _pos << std::endl;
	//if (!skipWhiteSpaceLines()) std::cout << "hSC !skipWhiteSpaceLines() = true" << std::endl;
	//if (handleConfig()) std::cout << "hSC handleConfig() = true" << std::endl;
	//else std::cout << "hSC handleConfig failed, _error: " << _error << std::endl;
	// ~ while (!skipWhiteSpaceLines())
	// ~	if (!handleConfig() && !ws_checkend(_line))
	// ~		return (std::cout << "hSC _line: " << _line << " _error: " << _error << std::endl, _error = "Section " + _section + " end missing", false);
	// *while (handleConfig() || !skipWhiteSpaceLines())
	// *		if (!ws_checkend(_line)) // ! CONTINUE FROM HERE
	// *			return (std::cout << "hSC ws_checkend failed, _line: " << _line << " _error: " << _error << std::endl, _error = "Section " + _section + " end missing", false);
	_pos = _line.size();

	while (skipWhiteSpaceLines() || handleConfig() || !ws_checkend(_line))
		;
	//	return (std::cout << "hSC loop failed at _line: " << _line << std::endl, false);
	//^while (!skipWhiteSpaceLines() && handleConfig())
	//^	std::cout << "hSC _line handled: " << _line << std::endl;
	if (!ws_checkend(_line)) { // ^ std::getline(_cfile, _line) || 
		std::cout << "hSC _line at end: " << _line << std::endl;
		return (_error = "Section end for section " + _section + " is missing!", false);
	}
	std::cout << "hSC exiting, _config_sections size: " << _config_sections.size() << std::endl;
	return (_section = topsection, true);
}

// ~ called by handleConfig, does not modify _pos
// checks syntax for _line against _config_defaults at index
// * seems to work, did some prelim testing
bool ConfigParser::checkSyntax(const size_t index) { std::cout << "checkSyntax called, index:  " << index << "line at index: " << _config_defaults.at(index) << " _line: " << _line << std::endl;
	size_t wordsize = ws_getword(_config_defaults.at(index)).size();
	if (wordsize == 0 || wordsize == std::string::npos)
		return (_error = "Unexpected syntax in config defaults!", false);
	size_t cfg_pos = wordsize + _pos;
	std::cout << "break ws: " << wordsize << "index: " << index << std::endl;
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
				while (!ws_endl(_line, cfg_index) && ws_wspace(_line.at(cfg_index)))
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
// returns true if reached end of config file, false if reached parseable content
bool ConfigParser::skipWhiteSpaceLines() {
	while (skipWhiteSpace()) {
		_pos = 0;
		if (!std::getline(_cfile, _line))
			return true;
	}
	return false;
}

void ConfigParser::printCS() {
	for (size_t i = 0; i < _config_sections.size(); i++)
		_config_sections.at(i).printAll();
	std::cout << "END" << std::endl;
}