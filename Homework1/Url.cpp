#include "Url.h"
#include <iostream>
Url::Url(string urlInput) {

	string url = urlInput;

	// scheme://host[:port][/path][?query][#fragment] 

	// find # -> remove 
	// find ? -> query -> remove
	// find / -> path -> remove
	// find : -> port -> remove -> host


	printf("----------------------------");
	printf("url: %s\n", url.c_str());

	// find # -> remove 
	url = url.substr(0, url.find('#', 0));
	printf("url: %s\n", url.c_str());


	// find ? -> query -> remove
	this->query = url.substr(url.find('?') + 1);
	url = url.substr(0, url.find('?', 0));

	printf("query: %s\n", query.c_str());
	printf("url: %s\n", url.c_str());


	// find / -> path -> remove
	this->path = url.substr(url.find_last_of('/') + 1);
	url = url.substr(0, url.find_last_of('/'));

	printf("path: %s\n", path.c_str());
	printf("url: %s\n", url.c_str());


	// find : -> port -> remove
	string portStr = url.substr(url.find_last_of(':') + 1);
	this->port = stoi(portStr);
	url = url.substr(0, url.find_last_of(':'));

	printf("port: %s\n", portStr.c_str());
	printf("port int: %i\n", port);
	printf("url: %s\n", url.c_str());

	// get scheme -> remove
	string scheme = url.substr(0, 7);
	if (scheme != "http://") {
		printf("Scheme not formatted correctly: %s", urlInput);
	}
	url = url.substr(7);

	// get host
	this->host = url;


	printf("----------------------------");

}
