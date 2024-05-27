#ifndef WS_SERVER_HPP
# define WS_SERVER_HPP

#include <cstring>
#include "Location.hpp"

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
		Server(std::string listen);

		bool addServerName(std::string name);
		bool errorPage(std::string file_path);
		bool clientMaxBodySize(size_t size);

		bool addLocation(Location location);

		const std::string listen_name;
		const size_t port;
		const std::string error_page;
};

#endif