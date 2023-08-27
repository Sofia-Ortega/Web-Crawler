#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using std::string;

class Socket
{
private:
	const int BUFFER_SIZE = 1024;

	char* buffer;
	int size;
	int capacity;

	char* url;

	SOCKET sock;

	string formatGetRequest(string host);
	void resizeBuffer();

public:
	Socket(char* address);
	~Socket();
	void Read(void);

};

