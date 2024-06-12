#ifndef WS_REQUEST_HPP
# define WS_REQUEST_HPP

# include <string>

# define REQ_GET 1
# define REQ_POST 2
# define REQ_DEL 3

class Request {
	public:
		Request();
		Request(const int method, const std::string host, const int port, const std::string path);
		Request(const std::string request_string);
		~Request();

		const int& getType();
		const std::string &getPath();

		Request &operator=(const Request assign);

		const std::string _host;
		const size_t _port;
		const std::string _path;
		const int _method;
		// 1 GET 2 POST 3 DELETE

	// private:
	// 	std::string _host;
	// 	int _port;
	// 	std::string _path;
	// 	int _method;
	// 	// 1 GET 2 POST 3 DELETE
};

#endif