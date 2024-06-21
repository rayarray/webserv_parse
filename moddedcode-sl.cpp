// ! added by rleskine
size_t getServer(std::vector<std::pair<int, int> > const &server_index, int fd) {
	size_t index = 0;
	for (; index < server_index.size(); index++) {
		if (server_index.at(index).first == fd) break;
	}
	if (index == server_index.size())
		throw std::runtime_error("Manager::handleGet couldn't match to server");
	return index;
}

// ! added by rleskine
void Manager::handleGet2(std::string receivedData, std::vector<struct pollfd> fds, int i) {
	size_t pos = receivedData.find(' ') + 2; // ! + 1 if leading slash
	std::string filepath = receivedData.substr(pos, receivedData.find(' ', pos) - pos);
	if (filepath.find('?') != std::string::npos)
		filepath = filepath.substr(0, filepath.find('?') + 1);
	Server &server = serverList.at(serverIndex.at(getServer(serverIndex, fds[i].fd)).second);
	server.log(receivedData);
	if (filepath.find(server.getCGIExt()) != std::string::npos)
		return (handleCGI(receivedData, fds, i));
	std::string response = server.buildHTTPResponse(filepath, "");
	if (server.getClientBodySize() && server.getClientBodySize() < response.size()) {
		std::string const body("ERROR 413 Request Entity Too Large");
		response = server.makeHeader(413, body.size());
		response.append(body);
	}
	send(fds[i].fd, response.c_str(), response.length(), 0);
}

// ! added by rleskine, not functional yet
// void Manager::handleGetParser(std::string request_data, std::vector<struct pollfd> fds, int i) {
// 	size_t pos = request_data.find(' ') + 1;
// 	std::string filepath = request_data.substr(pos, request_data.find(' ', pos) - pos);
// 	if (filepath.find('?') != std::string::npos)
// 		filepath = filepath.substr(0, filepath.find('?') + 1);
// 	Server &server = serverList.at(serverIndex.at(getServer(serverIndex, fds[i].fd)).second);
// 	server.log(request_data);
// 	Response response = server.resolveRequest(Request(REQ_GET, "todo", 0, filepath));
// 	if (response.getType() == -100)
// 		throw std::runtime_error("Server returned invalid request!");
// 	else if (response.getType() == -2)		// ! CGI
// 		return (handleCGI());
// 	else if (response.getType() == -1 || (response.getType() > 0 && response.get)) {	// ! File
// 	}

// }
