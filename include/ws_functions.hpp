#ifndef WS_WS_FUNCTIONS_HPP
# define WS_WS_FUNCTIONS_HPP
# include <vector>
# include <string>

inline bool ws_wspace(char c) { return (c == ' ' || c == '\t'); }

inline bool ws_checkword(const std::string &s, const std::vector<const std::string> &list) {
	for (const std::string& word : list) {
		if (s.find(word) == 0)
			return true;
	}
	return false;
}

#endif