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
	
	Url(string urlInput);
	

};

