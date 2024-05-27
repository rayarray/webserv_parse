#ifndef WS_REQUEST_HPP
# define WS_REQUEST_HPP

# include <cstring>

// request class that tells webserv how to resolve the request
// returned from Server class

// this class can handle file reading and CGI execution as well, if so desired
// but im coding parser first - rleskine 27.5

class Request {
	public:
		Request(int responsecode);
		Request(std::string filepath);
		Request(CGIExtension &ext, std::string execpath); 
			// CGIExtension not implemented, can also be deduced from suffix of file name (execpath)
		~Request();

		int& getType();
		

	private:
		int responsecode;
			// -2 if CGI, -1 if file, positive int if http
		std::string path;
		CGIExtension &ext; 
			// CGIExtension not implemented, maybe unnecessary (see above comment)
};

#endif