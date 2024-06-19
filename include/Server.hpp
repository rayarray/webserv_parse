#ifndef WS_SERVER_HPP
# define WS_SERVER_HPP

#include <cstring>
#include <vector>
#include <map>
#include "ConfigSection.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server : public ConfigSection {
	public:
		Server();

		void initialize();
		bool addServerName(const std::string name);
		bool addErrorPage(const std::string nbr, const std::string file_path);
		void addLocation(Location location);

		// accessed after setup
		bool matchRequest(const Request &request);
		bool matchPort(const size_t port);
		Response resolveRequest(const Request &request);
		std::string getErrorPage(const size_t page_num);

	private:
		size_t _max_client_body_size;
		std::vector<size_t> _ports;
		std::vector<std::string> _server_names;
		std::map<size_t, std::string> _error_pages;
		std::vector<Location> _locations;
	public:
		class ServerException : public std::exception {
			public:
				ServerException(std::string const msg) : _msg(msg) {}
				const char *what() const noexcept override { return _msg.c_str(); }
			private:
			const std::string _msg;
		};
	public: //debug
		void printData();
		std::string const printId() const;
};

#endif