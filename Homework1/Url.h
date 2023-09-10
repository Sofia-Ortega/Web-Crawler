#pragma once

#include <stdio.h>
#include <string>
using std::string;


#define MAX_HOST_LEN		256
#define MAX_URL_LEN			2048
#define MAX_REQUEST_LEN		2048

class Url
{
private:

public:
	string scheme;
	string host; 
	int port;
	string path;
	string query;
	
	string request;
	string baseUrl;

	Url(string urlInput);

	Url();
	Url(const Url& other);
	Url& operator=(const Url& other);

	char* getAddress();
	char* getBaseUrl();


	
};

