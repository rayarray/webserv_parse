#ifndef WS_SERVER_HPP
# define WS_SERVER_HPP

#include <cstring>
#include <vector>
#include <map>
#include "ConfigSection.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"

// listen one.example.com
// server_name one.example.com www.one.example.com
// error_page 404 /404.html *USES LOCATION*
// client_max_body_size
// location / { }

class Server : public ConfigSection {
	public:
		Server(const std::string listen, const size_t port);

		void initialize();
		bool addServerName(const std::string name);
		bool addErrorPage(const std::string nbr, const std::string file_path);
		//bool setClientMaxBodySize(const size_t size);
		bool addLocation();
		void addLocation(Location location);

		// accessed after setup
		bool matchRequest(const std::string server_name, const size_t port); //todo
		bool matchRequest(const Request &request);
		Response resolveRequest(const std::string request); //todo
		Response resolveRequest(const Request &request);

		//debug
		void printData();

	private:
		bool matchLocation(const std::string path);

		const std::string _listen_name;
		const size_t _port;
		size_t _max_client_body_size;
		std::vector<std::string> _server_names;
		std::map<int, std::string> _error_pages;
		std::vector<Location> _locations;
};

#endif