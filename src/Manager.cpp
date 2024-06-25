#include "Manager.hpp"

Manager::Manager(std::string config_file) : _responses("Responses") {
    ConfigParser parser(config_file);
    if (!parser.startParse())
        throw std::logic_error("Parsing failed to start");
    while (!parser.endParse()) {
        _servers.push_back(parser.getServer());
}


