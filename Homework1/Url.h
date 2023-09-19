/*
*
* Name: Sofia Ortega
* Class: CSCE 463-500
* Semester: Fall 2023
*
*/

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
	char* scheme;
	char* host; 
	int port;
	char* path;
	char* query;
	
	char* request;
	char* baseUrl;

	Url(char* urlInput);

	Url();
	~Url();
	Url(const Url& other);
	Url& operator=(const Url& other);

	char* getAddress();
	char* getBaseUrl();


	
};

