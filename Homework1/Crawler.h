#pragma once

#include "stdafx.h"
#include <queue>


class Crawler
{
private:
	std::queue<char*> q;

	HANDLE mutexQueue;
	HANDLE mutexStats;
	HANDLE eventQuit;

	int maxThreadNum;
	int finishedThreads;

	int originalQueueSize;
	int uniqueHosts;
	int numOfDnsLookups;
	int uniqueIps;
	int passedRobots;
	int crawledUrls;
	int totalLinks;

	int robotsAttempted;


	int bytesDownloaded;


	int status200;
	int status300;
	int status400;
	int status500;
	int statusOther;


	void queueLock();
	void queueUnlock();

	void statsLock();
	void statsUnlock();
public:
	Crawler(int numOfThreads);
	~Crawler();

	void ReadFile(const char* inputFileName);
	void Run();
	void printStats();
	void printSummary();
	void quitStatsThread();
};

