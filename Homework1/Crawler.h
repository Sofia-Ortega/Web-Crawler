#pragma once

#include "stdafx.h"
#include <queue>


class Crawler
{
private:
	std::queue<Url> q;

	HANDLE* handles;
	HANDLE mutex;
	HANDLE eventQuit;


	int originalQueueSize;
	int uniqueHosts;
	int uniqueIps;
	int passedRobots;
	int crawledUrls;
	int totalLinks;

	void printSummary();
public:
	Crawler();
	~Crawler();

	void ReadFile(string inputFileName);
	void Run();
	void printStats();
	void quitStatsThread();
};

