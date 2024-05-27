#include <iostream>
#include "ConfigParser.hpp"

// main for testing configparser, reads default.conf in current directory

int main(void) {
	ConfigParser conf("default.conf");
	std::cout << std::boolalpha << "ConfigParser.startParse() = " << conf.startParse() << std::endl;
	std::cout << "MAIN: _line: [" << conf._error << "]" << std::endl;
}