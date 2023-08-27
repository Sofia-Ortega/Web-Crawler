#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using std::string;

class Socket
{
private:
	int capacity;
	int size;
	char* buffer;

	char* url;

	SOCKET sock;

	string formatGetRequest(string host);

public:
	Socket(char* address);
	~Socket();
	void Read(void);

};

