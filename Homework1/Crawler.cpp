#include "Crawler.h"


Crawler::Crawler() : handles(nullptr), originalQueueSize(0), uniqueHosts(0), uniqueIps(0), passedRobots(0), crawledUrls(0), totalLinks(0) {

	mutex = CreateMutex(NULL, 0, NULL);

	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

}

Crawler::~Crawler() {
	WSACleanup();
}

void Crawler::ReadFile(string inputFileName) {

	std::ifstream inputFile(inputFileName);
	
	if (!inputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return;
	}


	inputFile.seekg(0, inputFile.end);
	int fileSize = inputFile.tellg();

	printf("Opened %s with size %i", inputFileName.c_str(), fileSize);

	inputFile.seekg(0, inputFile.beg);

	string link;
	while (std::getline(inputFile, link)) {
		try {
			Url url(link);
			q.push(url);
		}
		catch (const std::exception& e) {
			printf("[ERROR] %s", e.what());
		}
	}

	inputFile.close();

	originalQueueSize = q.size();
	uniqueHosts = 0;
	uniqueIps = 0;
}



void Crawler::Run() {

	// sync 
	WaitForSingleObject(mutex, INFINITE); // wait for mutex
	printf("thread Run %d started\n", GetCurrentThreadId()); // print inside mutex to avoid garbage



	Sleep(1000); // critical section
	ReleaseMutex(mutex); // release mutex

	// sync
	// pop queue
	// printf("Parsing url");
	// release mutex




	






}


void Crawler::printStats() {
	float Q = q.size();
	int E = originalQueueSize - q.size();
	int H = uniqueHosts;
	int I = uniqueIps;
	int R = passedRobots;
	int C = crawledUrls;
	int L = totalLinks;

	

}


void Crawler::printSummary() {
	int numOfDns = 0;

	printf("Extracted %i URLs @ %i/s\n", originalQueueSize, 0);
	printf("Looked up %i DNS names @ %i/s\n", numOfDns, 0);
	printf("Attempted %i site robots @ %i/s", 0, 0);
	printf("Crawled %i pages @ %i/s (0.23MB)", 0, 0);
	printf("Parsed %i links @ %i/s", 0, 0);
	printf("HTTP codes: 2xx = %i, 3xx = %i, 4xx = %i, 5xx = %i, other = %i", 5, 4, 0, 0, 0);
}

