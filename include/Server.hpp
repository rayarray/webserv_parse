#ifndef WS_SERVER_HPP
# define WS_SERVER_HPP

#include <cstring>
#include "Location.hpp"
#include "Request.hpp"

// listen x
// server_name y z
// error_page
// client_max_body_size
// location /
//		http_methods GET POST DEL
//		http_redirection
//		root
//		directory_list y/n
//		default_index
//		request_method
//		cgi_extension .php  .py { }

class Server {
	public:
		Server(const std::string listen, const size_t port);

		// methods for setting up
		bool addServerName(std::string name);
		bool errorPage(std::string file_path);
		bool clientMaxBodySize(size_t size);

		bool addLocation(Location location);

		// accessed after setup
		bool matchRequest(const std::string server_name, const size_t port);
		Request resolveRequest(const std::string request);

		const std::string _listen_name;
		const size_t _port;
		const std::string _error_page;
};

#endif