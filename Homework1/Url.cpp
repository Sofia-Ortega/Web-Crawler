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

	baseUrl = urlInput;
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
	size_t forwardSlash = url.find('/');
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
	this->request = path;

	if (query != "") {
		request += "?" + query;
	}

	if (request.at(request.size() - 1) != '/') {
		request += "/";
	}


	printf("host %s, port %i, request %s\n", host.c_str(), port, request.c_str());


}

Url::Url() {}

Url::Url(const Url& other) : scheme(other.scheme), host(other.host), port(other.port), path(other.path), query(other.query), request(other.request), baseUrl(other.baseUrl) {}

Url& Url::operator=(const Url& other) {
	if (this != &other) {
		scheme = other.scheme;
		host = other.host;
		port = other.port;
		path = other.path;
		query = other.query;
		request = other.request;
		baseUrl = other.baseUrl;
	}

	return *this;
} 

char* Url::getAddress() {

	size_t length = host.size();
	char* res = new char[length];

	memset(res, '\0', length);
	strcpy(res, host.c_str());

	return res;

}

char* Url::getBaseUrl() {

	size_t length = baseUrl.size();
	char* res = new char[length];

	memset(res, '\0', length);
	strcpy(res, baseUrl.c_str());

	return res;

}
