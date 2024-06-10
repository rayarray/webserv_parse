#include "Location.hpp"

Location::Location(const std::string path) : ConfigSection("location"), _path(path) {}