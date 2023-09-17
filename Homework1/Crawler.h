#pragma once

#include "stdafx.h"
#include <queue>


class Crawler
{
private:
	std::queue<Url> q;

	HANDLE mutexQueue;
	HANDLE mutexStats;
	HANDLE threadSemaphore;
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


	void printSummary();

	void queueLock();
	void queueUnlock();

	void statsLock();
	void statsUnlock();
public:
	Crawler(int numOfThreads);
	~Crawler();

	void ReadFile(string inputFileName);
	void Run();
	void printStats();
	void quitStatsThread();
};

