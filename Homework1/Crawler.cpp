#include "Crawler.h"


Crawler::Crawler() : handles(nullptr), originalQueueSize(0), uniqueHosts(0), uniqueIps(0), passedRobots(0), crawledUrls(0), totalLinks(0) {

	// initial state NOT locked
	mutex = CreateMutex(NULL, 0, NULL);
	// manual reset, initial state = not signaled
	eventQuit = CreateEvent(NULL, true, false, NULL);

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

	while (!q.empty()) {
		WaitForSingleObject(mutex, INFINITE); // wait for mutex

		if (!q.empty()) {

			printf("thread Run %d started running with %s\n", GetCurrentThreadId(), q.front().baseUrl.c_str()); // print inside mutex to avoid garbage
			q.pop();
		}


		ReleaseMutex(mutex); // release mutex


		// read 
		Sleep(8000); // critical section

	}

	







	






}


void Crawler::printStats() {
	while (true) {
		DWORD waitResult = WaitForSingleObject(eventQuit, 3000); // Wait for 3 seconds or until the event is signaled
		if (waitResult == WAIT_OBJECT_0) {
			break;
		}

		float Q = q.size();
		int E = originalQueueSize - q.size();
		int H = uniqueHosts;
		int I = uniqueIps;
		int R = passedRobots;
		int C = crawledUrls;
		int L = totalLinks;

		printf("Stat Thread:\n");
		printf("QueueSize: %f \n\n", Q);
	}

}

void Crawler::quitStatsThread() {
	SetEvent(eventQuit);

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

