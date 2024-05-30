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
#endif