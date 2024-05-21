#ifndef WS_LOCATION_HPP
# define WS_LOCATION_HPP

#include <cstring>

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

class Location {
	public:
		Location(std::string path);

		bool setMethods(bool get, bool post, bool delete);
		bool setRedirection();
		bool setRoot(std::string rootpath);
		bool setDirectoryList(bool yesno);
		bool setDefaultIndex(std::string indexfilepath);
		bool setRequestMethod();
		bool setCGI();
		
		const std::string path;

		private:
		
};

#endif