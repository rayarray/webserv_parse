#ifndef WS_WS_FUNCTIONS_HPP
# define WS_WS_FUNCTIONS_HPP
# include <vector>
# include <string>

inline bool ws_wspace(char c) { return (c == ' ' || c == '\t'); }

inline bool ws_checkword(const std::string &s, const std::vector<std::string> &list) {
	for (const std::string& word : list) {
		if (s.find(word) == 0)
			return true;
	}
	return false;
}

// checks if word is found in vector, searches only until first char in vector elem is not uppercase
inline bool ws_checkword_lower(const std::string &s, const std::vector<std::string> &list) {
	for (const std::string& word : list) {
		if (std::isupper(s.at(0))) 
			return false;
		if (s.find(word) == 0)
			return true;
	}
	return false;
}

inline std::string ws_toupper(const std::string &s) {
	std::string s_up(s);
	std::transform(s_up.begin(), s_up.end(), s_up.begin(), ::toupper);
	return s_up;
}

#endif