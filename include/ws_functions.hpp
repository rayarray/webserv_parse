#ifndef WS_WS_FUNCTIONS_HPP
# define WS_WS_FUNCTIONS_HPP
# include <vector>
# include <string>

inline bool ws_wspace(char c) { return (c == ' ' || c == '\t'); }

inline bool ws_checkword(const std::string &s, const std::vector<std::string> &list, size_t &index) {
	for (index = 0; index < list.size(); index++) {
		if (s.find(list.at(index))) {
			return true;
		}
	}
	return false;
}

// checks if word is found in vector, searches only until first char in vector elem is not uppercase
inline bool ws_checkword_lower(const std::string &s, const std::vector<std::string> &list, size_t &index) {
	for (index = 0; index < list.size() && !std::isupper(list.at(index).at(0)); index++) {
		if (s.find(list.at(index))) {
			return true;
		}
	}
	return false;
}

inline std::string ws_toupper(const std::string &s) {
	std::string s_up(s);
	std::transform(s_up.begin(), s_up.end(), s_up.begin(), ::toupper);
	return s_up;
}

//checks if endline reached (# counts as endline)
inline bool ws_endl(const std::string &s, const size_t pos) {
	if (pos > s.size() || s.at(pos) == '#')
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