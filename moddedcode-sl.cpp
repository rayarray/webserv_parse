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
	size_t pos = receivedData.find(' ');
	std::string filepath = receivedData.substr(pos + 1, receivedData.find(' ', pos + 1));
	if (filepath.find('?') != std::string::npos)
		filepath = filepath.substr(0, filepath.find('?') + 1);
	
}
