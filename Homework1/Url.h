#pragma once

#include <stdio.h>
#include <string>
using std::string;

class Url
{
public:
	string scheme;
	string host; 
	int port;
	string path;
	string query;
	
	string request;

	Url(string urlInput);

	Url();
	Url(const Url& other);
	Url& operator=(const Url& other);


	
};

