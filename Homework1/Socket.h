#pragma once

#include <iostream>
#include <string>
#include <set>
#include <stdio.h>
#include <windows.h>
#include <ctime>

#include "Url.h"
#include "HTMLParserBase.h"

using std::string;
using std::set;

class Socket
{
private:
	const int BUFFER_SIZE = 1024;

	static set<DWORD> seenIPs;
	static set<string> seenHosts;

	char* buffer;
	int size;
	int capacity;

	bool robotsNotAllowed;

	Url url;

	SOCKET sock;

	string formatGetRequest();
	string formatRobotRequest();

	int getStatusCode();

	void resizeBuffer();
	void clearBuffer();

	clock_t startTime;

	void startClock();
	int endClock();

	int readRequestIntoBuffer(string getRequest, SOCKET mySock);

public:
	Socket(const Url& urlInput);
	~Socket();
	void Read(void);
	void Parse(void);

};

