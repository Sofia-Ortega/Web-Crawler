/*
*
* Name: Sofia Ortega
* Class: CSCE 463-500
* Semester: Fall 2023
*
*/


#include "Url.h"
#include <iostream>


Url::Url(char* urlInput) : request(nullptr), baseUrl(nullptr) {

	// scheme://host[:port][/path][?query][#fragment] 

	// get scheme -> remove
	// find # -> remove 
	// find ? -> query -> remove
	// find / -> path -> remove
	// find : -> port -> remove -> host


	// printf("URL: %s\n", urlInput);
	// printf("\tParsing URL... ");

	baseUrl = urlInput;
	char* url = urlInput;

	if (strlen(url) > MAX_URL_LEN) {
		throw std::exception(" URL Length too long");
		return;
	}

	// get scheme -> remove
	if (strncmp(url, "http://", 7) != 0) {
		throw std::invalid_argument("failed with invalid scheme");
	}

	 url += 7;


	// Hashtag
	// find # -> remove 
	char* hashTag = strrchr(url, '#');
	if (hashTag != nullptr) {
		*hashTag = '\0';
	}


	// Query
	// find ? -> query -> remove
	char* questionMark = strrchr(url, '?');
	if (questionMark != nullptr) {
		this->query = questionMark + 1;
		*questionMark = '\0'; // truncate
	}
	else {
		this->query = nullptr;
	}


	// Path
	// find / -> path -> remove
	char* forwardSlash = strchr(url, '/');
	if (forwardSlash != nullptr) {
		this->path = forwardSlash + 1; // FIXME must add forward slash in request
		*forwardSlash = '\0';
	}
	else {
		this->path = nullptr;
	}

	// Port
	// find : -> port -> remove
	char* twoDots = strrchr(url, ':');
	if (twoDots != nullptr) {
		char* portStr = twoDots + 1;

		// convert port to int
		try {
			this->port = std::stoi(portStr);
		}
		catch (const std::exception& err) {
			throw std::invalid_argument("failed with invalid port");
		}


		if (this->port == 0) {
			throw std::invalid_argument("failed with invalid port");
		}

		*twoDots = '\0';
	}
	else {
		this->port = 80;
	}


	// Host
	this->host = url;
	if (strlen(host) > MAX_HOST_LEN) {
		throw std::exception("Host URL Length too long");
	}


	// Request
	size_t pathLength = 0;
	if (path)
		pathLength = strlen(path);

	size_t queryLength = 0;
	if (query)
		queryLength = strlen(query);

	size_t requestLength = pathLength + 2; // for null terminated string + extra /
	requestLength += queryLength + 1;

	if (requestLength > MAX_REQUEST_LEN) {
		throw std::exception("Request length too long");
	}

	this->request = new char[requestLength];

	request[0] = '/';

	if (path)
		strcpy(request + 1, path);


	if (query) {
		strcat(request, "?");
		strcat(request, query);
	}
	

	// printf("host %s, port %i, path %s, request %s\n", host, port, path, this->request);

	printf("URL: %s\n", baseUrl);
	printf("host %s, port %i, path %s, request %s, length: %i\n\n", host, port, path, this->request, strlen(request));


}

Url::Url() : request(nullptr), baseUrl(nullptr) {}


void Url::deleteUrl() {
	if (request)
		delete request;

	if (baseUrl)
		delete baseUrl;
	request = nullptr;
	baseUrl = nullptr;
}

Url::Url(Url&& other) : scheme(other.scheme), host(other.host), port(other.port), path(other.path), query(other.query), request(other.request), baseUrl(other.baseUrl) {
	other.request = nullptr;
	other.baseUrl = nullptr;
}

Url& Url::operator=(Url&& other) {
	if (this != &other) {

		deleteUrl();

		scheme = other.scheme;
		host = other.host;
		port = other.port;
		path = other.path;
		query = other.query;
		request = other.request;
		baseUrl = other.baseUrl;

		other.request = nullptr;
		other.baseUrl = nullptr;

	}

	return *this;
} 

char* Url::getAddress() {
	return host;
}

char* Url::getBaseUrl() {
	return baseUrl;

}
