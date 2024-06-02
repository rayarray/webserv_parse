#ifndef WS_WS_FUNCTIONS_HPP
# define WS_WS_FUNCTIONS_HPP
# include <vector>
# include <string>
#include <iostream> // debug

inline bool ws_wspace(char c) { return (c == ' ' || c == '\t'); }

inline const std::string ws_getword(const std::string &s) {
	size_t end = 0;
	if (s.empty() || !std::isalpha(s.at(0)))
		return "";
	while (end < s.size() && std::isalpha(s.at(end)) && !ws_wspace(s.at(end)))
		end++;
	return s.substr(0, end);
}

inline bool ws_checkword(const std::string &s, const std::vector<std::string> &list, size_t &index) {
	for (index = 0; index < list.size(); index++) {
		if (s.find(list.at(index)) == 0) {
			return true;
		}
	}
	return false;
}

// checks if word is found in vector, searches only until first char in vector elem is not uppercase
inline bool ws_checkword_lower(const std::string &s, const std::vector<std::string> &list, size_t &index) {
	//std::cout << "ws_cw_l: " << s << std::endl;
	for (index = 0; index < list.size() && !std::isupper(list.at(index).at(0)); index++) {
		//std::cout << "ws_cw_l iterating elem: " << ws_getword(list.at(index)) << " find result: " << s.find(ws_getword(list.at(index))) << std::endl;
		if (s.find(ws_getword(list.at(index))) == 0)
			return true;
	}
	//return (std::cout << "ws_cw_l returning false" << std::endl, false);
	return false;
}

inline std::string ws_toupper(const std::string &s) {
	std::string s_up(s);
	std::transform(s_up.begin(), s_up.end(), s_up.begin(), ::toupper);
	return s_up;
}

//checks if endline reached (# counts as endline)
inline bool ws_endl(const std::string &s, const size_t pos) {
	if (pos >= s.size() || s.at(pos) == '#')
		return true;
	return false;
}

// return size of string, excluding comments (comments start with #)
inline size_t ws_size(const std::string &s) {
	size_t end = 0;
	while (end < s.size() && s.at(end) != '#')
		end++;
	return end;
}

// get num:th argument from config defaults line
inline char ws_getarg(size_t num, std::string s) {
	size_t pos = 0;
	while (pos < s.size() && !ws_wspace(s.at(pos)))
		pos++;
	while (pos < s.size() && ws_wspace(s.at(pos)))
		pos++;
	while (num > 1) {
		if (pos < s.size() && std::isupper(s.at(pos)) && ++pos) {
			if (pos + 1 == s.size() && s.at(pos - 1) == s.at(pos) && s.at(pos) != 'S')
				return s.at(pos);
			else if (pos == s.size())
				return 0;
			else if (pos < s.size() && !ws_wspace(s.at(pos)))
				throw std::runtime_error("Invalid config default syntax: ws_getarg");
		}
		pos++;
		num--;
	}
	return s.at(pos);
}

// returns how many arguments a keyword takes, does not count repeats
inline size_t ws_getarglen(std::string s) {
	size_t pos = 0;
	size_t len = 0;
	while (pos < s.size() && !ws_wspace(s.at(pos)))
		pos++;
	while (pos < s.size() && ws_wspace(s.at(pos)))
		pos++;
	if (pos == s.size()) return 0;
	while (++len && pos < s.size()) {
		if (!std::isupper(s.at(pos)))
			throw std::runtime_error("Invalid config default syntax:1 ws_getarglen");
		pos++;
		if (ws_endl(s, pos) || (s.at(pos - 1) == s.at(pos) && s.at(pos) != 'S' && ws_endl(s, pos + 1)))
			return len;
		else if (!ws_wspace(s.at(pos)))
			throw std::runtime_error("Invalid config default syntax:2 ws_getarglen");
		pos++;
	}
	throw std::runtime_error("Error at ws_getarglen");
}

#endif