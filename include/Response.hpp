#ifndef WS_RESPONSE_HPP
# define WS_RESPONSE_HPP

# include <string>

// request class that tells webserv how to resolve the request
// returned from Server class

// this class can handle file reading and CGI execution as well, if so desired
// but im coding parser first - rleskine 27.5

# define REQ_FILE -1
# define REQ_CGI -2
# define REQ_INVALID -100

class Response {
	public:
		Response();
		Response(const int code, const std::string filepath);
		Response(const std::string filepath);
		//Response(CGIExtension &ext, std::string execpath); 
			// CGIExtension not implemented, can also be deduced from suffix of file name (execpath)
		~Response();

		const int& getType();
		const std::string &getPath();

		Response &operator=(const Response assign);

	private:
		int _responsecode;
			// -2 if CGI, -1 if file, positive int if http
		std::string _path;
		//CGIExtension &ext; 
			// CGIExtension not implemented, maybe unnecessary (see above comment)
};

#endif