#include "Url.h"
#include <iostream>
Url::Url(string urlInput) {

	// scheme://host[:port][/path][?query][#fragment] 

	// get scheme -> remove
	// find # -> remove 
	// find ? -> query -> remove
	// find / -> path -> remove
	// find : -> port -> remove -> host


	printf("URL: %s\n", urlInput.c_str());
	printf("\tParsing URL... ");

	string url = urlInput;


	// get scheme -> remove
	string scheme = url.substr(0, 7);
	if (scheme != "http://") {
		throw std::invalid_argument("failed with invalid scheme");
	}

	url = url.substr(7);



	// Hashtag
	// find # -> remove 
	size_t hashTag = url.find_last_of('#');
	if (hashTag != string::npos) {
		url = url.substr(0, hashTag);
	}


	// Query
	// find ? -> query -> remove
	size_t questionMark = url.find_last_of('?');
	if (questionMark != std::string::npos) {
		this->query = url.substr(questionMark + 1);
		url = url.substr(0, questionMark);
	}
	else {
		this->query = "";
	}


	// Path
	// find / -> path -> remove
	size_t forwardSlash = url.find_last_of('/');
	if (forwardSlash != std::string::npos) {
		this->path = url.substr(forwardSlash);
		url = url.substr(0, forwardSlash);
	}
	else {
		this->path = "/";
	}

	// Port
	// find : -> port -> remove
	size_t twoDots = url.find_last_of(':');
	if (twoDots != std::string::npos) {
		string portStr = url.substr(twoDots + 1);

		// convert port to int
		try {
			this->port = stoi(portStr);
		}
		catch (std::exception& err) {
			throw std::invalid_argument("failed with invalid port");
		}


		if (this->port == 0) {
			throw std::invalid_argument("failed with invalid port");
		}

		url = url.substr(0, twoDots);
	}
	else {
		this->port = 80;
	}


	// Host
	this->host = url;

	// combine
	this->request = path + query;

	printf("host %s, port %i, request %s\n", host.c_str(), port, request.c_str());

	/*
	printf("scheme: %s\n", scheme.c_str());
	printf("host: %s\n", host.c_str());
	printf("port: %i\n", port);
	printf("path: %s\n", path.c_str());
	printf("query: %s\n", query.c_str());

	printf("----------------------------\n");
	*/

}
