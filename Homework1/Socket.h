
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

using std::strcmp;
using std::set;
using std::string;


class Socket
{
private:

	// compare actual contents instead of addresses
	struct Comparator {
		bool operator()(const char* a, const char* b) const {
			return strcmp(a, b) < 0;
		}
	};

	const int BUFFER_SIZE = 1024;


	char* buffer;
	int size;
	int capacity;

	bool readReady;

	bool robotsNotAllowed;




	SOCKET sock;

	char* formatGetRequest();
	char* formatRobotRequest();


	void resizeBuffer();
	void clearBuffer();

	clock_t startTime;

	void startClock();
	int endClock();

	int getStatusCode();

	int readRequestIntoBuffer(char* getRequest, SOCKET mySock, int maxDownloadSize);

	static HANDLE mutexHosts;
	static HANDLE mutexIPs;

	void hostLock();
	void hostUnlock();

	void IPsLock();
	void IPsUnlock();

	

public:
	Url url;

	static set<DWORD> seenIPs;
	//static set<char*, Comparator> seenHosts;
	static set<string> seenHosts;


	int tamuLinksFound;

	short uniqueHost;
	short successfulDNSNum;
	short uniqueIp;
	short robotAttempted;
	short passedRobots;
	short crawledUrlSuccess;

	int bytesDownloaded;

	int numOfLinks;

	int statusCode;

	Socket(char* link);
	~Socket();
	void Read(void);
	void Parse(void);

};

