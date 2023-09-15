#pragma once

#include "stdafx.h"
#include <queue>


class Crawler
{
private:
	std::queue<Url> q;

	HANDLE* handles;
	HANDLE mutex;


	int originalQueueSize;
	int uniqueHosts;
	int uniqueIps;
	int passedRobots;
	int crawledUrls;
	int totalLinks;

	void printStats();
	void printSummary();
public:
	Crawler();
	~Crawler();

	void ReadFile(string inputFileName);
	void Run();
};

