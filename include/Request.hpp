#ifndef WS_REQUEST_HPP
# define WS_REQUEST_HPP

# include <string>

// request class that tells webserv how to resolve the request
// returned from Server class

// this class can handle file reading and CGI execution as well, if so desired
// but im coding parser first - rleskine 27.5

# define REQ_FILE -1
# define REQ_CGI -2
# define REQ_INVALID -100

class Request {
	public:
		Request();
		Request(int responsecode);
		Request(const std::string filepath);
		//Request(CGIExtension &ext, std::string execpath); 
			// CGIExtension not implemented, can also be deduced from suffix of file name (execpath)
		~Request();

		int& getType();
		const std::string &getPath();

		Request &operator=(const Request assign);

	private:
		const int _responsecode;
			// -2 if CGI, -1 if file, positive int if http
		const std::string _path;
		//CGIExtension &ext; 
			// CGIExtension not implemented, maybe unnecessary (see above comment)
};

#endif