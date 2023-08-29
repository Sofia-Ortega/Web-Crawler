#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <ctime>

#include "Url.h"
#include "HTMLParserBase.h"

using std::string;

class Socket
{
private:
	const int BUFFER_SIZE = 1024;

	char* buffer;
	int size;
	int capacity;

	Url url;

	SOCKET sock;

	string formatGetRequest();
	void resizeBuffer();

	clock_t startTime;

	void startClock();
	int endClock();

public:
	Socket(const Url& urlInput);
	~Socket();
	void Read(void);
	void Parse(void);

};

