
/*
*
* Name: Sofia Ortega
* Class: CSCE 463-500
* Semester: Fall 2023
*
*/



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


	void resizeBuffer();
	void clearBuffer();

	clock_t startTime;

	void startClock();
	int endClock();

	int getStatusCode();

	int readRequestIntoBuffer(string getRequest, SOCKET mySock, int maxDownloadSize);

public:
	short uniqueHost;
	short successfulDNSNum;
	short uniqueIp;
	short robotAttempted;
	short passedRobots;
	short crawledUrlSuccess;

	int bytesDownloaded;

	int numOfLinks;

	int statusCode;

	Socket(const Url& urlInput);
	~Socket();
	void Read(void);
	void Parse(void);

};

