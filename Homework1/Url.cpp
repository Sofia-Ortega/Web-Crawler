#include "Url.h"
#include <iostream>
Url::Url(string urlInput) {

	string url = urlInput;

	// scheme://host[:port][/path][?query][#fragment] 

	// get scheme -> remove
	// find # -> remove 
	// find ? -> query -> remove
	// find / -> path -> remove
	// find : -> port -> remove -> host


	printf("----------------------------\n");
	printf("Constructing link: %s\n\n", urlInput.c_str());

	// get scheme -> remove
	string scheme = url.substr(0, 7);
	if (scheme != "http://") {
		printf("Scheme not formatted correctly: %s", urlInput);
		throw std::invalid_argument("Scheme not 'http://'" + urlInput);
	}

	//printf("schemmmee\n");

	url = url.substr(7);


	// find # -> remove 
	size_t hashTag = url.find_last_of('#');
	if (hashTag != string::npos) {
		url = url.substr(0, hashTag);
	}

	//printf("hashhhhhhhh\n");

	// find ? -> query -> remove
	size_t questionMark = url.find_last_of('?');
	if (questionMark != std::string::npos) {
		this->query = url.substr(questionMark + 1);
		url = url.substr(0, questionMark);
	}
	else {
		this->query = "";
	}

	//printf("queryyyyyyyyy\n");

	// find / -> path -> remove
	size_t forwardSlash = url.find_last_of('/');
	if (forwardSlash != std::string::npos) {
		this->path = url.substr(forwardSlash + 1);
		url = url.substr(0, forwardSlash);
	}
	else {
		this->path = "/";
	}

	//printf("/////////////\n");

	// port
	// find : -> port -> remove
	size_t twoDots = url.find_last_of(':');
	if (twoDots != std::string::npos) {
		printf("two dots \n");
		string portStr = url.substr(twoDots + 1);
		this->port = stoi(portStr);
		url = url.substr(0, twoDots);

		if (this->port == 0) {
			throw std::invalid_argument("port must be non-zero");
		}
	}
	else {
		this->port = 80;
	}

	//printf(":::::::::::::\n");


	// get host
	this->host = url;


	printf("scheme: %s\n", scheme.c_str());
	printf("host: %s\n", host.c_str());
	printf("port: %i\n", port);
	printf("path: %s\n", path.c_str());
	printf("query: %s\n", query.c_str());

	printf("----------------------------\n");
	

}
