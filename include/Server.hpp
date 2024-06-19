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
		Server();

		void initialize();
		bool addServerName(const std::string name);
		bool addErrorPage(const std::string nbr, const std::string file_path);
		//bool setClientMaxBodySize(const size_t size);
		bool addLocation();
		void addLocation(Location location);

		// accessed after setup
		bool matchRequest(const std::string server_name, const size_t port); //todo
		bool matchRequest(const Request &request);
		//Response resolveRequest(const std::string request); //todo
		Response resolveRequest(const Request &request);
		std::string getErrorPage(const size_t page_num);

		//const std::string _listen_name;
	private:
		//bool matchLocation(const std::string path); // nonexisting
		bool matchPort(const size_t port);
		void addPort(const size_t port);

		//const size_t _port;
		size_t _max_client_body_size;
		std::vector<size_t> _ports;
		std::vector<std::string> _server_names;
		std::map<size_t, std::string> _error_pages;
		std::vector<Location> _locations;
	public: //debug function(s) and exception(s)
		void printData();
		std::string const printId() const;
		// friend std::ostream &operator<<(std::ostream& stream, const ::Server& server);
		class ServerException : public std::exception {
			public:
				ServerException(std::string const msg) : _msg(msg) {}
				const char *what() const noexcept override { return _msg.c_str(); }
			private:
			const std::string _msg;
		};
};

// std::ostream &operator<<(std::ostream& stream, const ::Server& server) {
// 	if (!server._server_names.empty())
// 		stream << server._server_names.at(0);
// 	for (size_t const &p : server._ports)
// 		stream << ":" << p;
// 	return stream;
// }

#endif