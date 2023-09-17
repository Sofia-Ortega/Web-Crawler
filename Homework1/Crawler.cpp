#include "Crawler.h"


Crawler::Crawler(int numOfThreads) : originalQueueSize(0), uniqueHosts(0), numOfDnsLookups(0), uniqueIps(0), passedRobots(0), crawledUrls(0), totalLinks(0) {
	this->maxThreadNum = numOfThreads;
	finishedThreads = 0;

	// initial state NOT locked
	mutexQueue = CreateMutex(NULL, 0, NULL);
	mutexQueue = CreateMutex(NULL, 0, NULL);
	
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
	CloseHandle(mutexQueue);
	CloseHandle(mutexStats);
	WSACleanup();
}

void Crawler::ReadFile(string inputFileName) {

	std::ifstream inputFile(inputFileName);
	
	if (!inputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return;
	}


	inputFile.seekg(0, inputFile.end);
	int fileSize = (int)inputFile.tellg();

	printf("Opened %s with size %i\n", inputFileName.c_str(), fileSize);

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

void Crawler::queueLock() {
	WaitForSingleObject(mutexQueue, INFINITE); // wait for mutex

}

void Crawler::queueUnlock() {
	ReleaseMutex(mutexQueue);
}

void Crawler::statsLock() {
	WaitForSingleObject(mutexStats, INFINITE); // wait for mutex

}

void Crawler::statsUnlock() {
	ReleaseMutex(mutexStats);

}

void Crawler::Run() {

	// sync 

	while (!q.empty()) {
		queueLock();

		Url url;
		if (!q.empty()) {

			url = q.front();
			printf("thread Run %d started running with %s\n", GetCurrentThreadId(), q.front().baseUrl.c_str()); // print inside mutex to avoid garbage
			q.pop();
		}

		
		queueUnlock();


		// read 
		
		try {
			Socket sock(url);
			sock.Read();

			statsLock();
			
			uniqueHosts += sock.uniqueHost;
			numOfDnsLookups += sock.successfulDNSNum;
			uniqueIps += sock.uniqueIp;
			passedRobots += sock.passedRobots;
			crawledUrls += sock.crawledUrlSuccess;

			totalLinks += sock.numOfLinks;

			statsUnlock();

			Sleep(2500);

		}
		catch (const std::exception& e) {
			if (e.what() != "Unknown exception")
				printf("[ERROR] %s", e.what());
		}
	}


	statsLock();
	finishedThreads += 1;
	statsUnlock();
}


void Crawler::printStats() {

	clock_t startTime = clock();

	while (true) {


		DWORD waitResult = WaitForSingleObject(eventQuit, 3000); // Wait for 3 seconds or until the event is signaled
		if (waitResult == WAIT_OBJECT_0) {
			break;
		}

		// lock mutex
		WaitForSingleObject(mutexQueue, INFINITE); // wait for mutex
		float Q = q.size();
		ReleaseMutex(mutexQueue);


		statsLock();
		int currNumOfActiveThreads = maxThreadNum - finishedThreads;
		if (currNumOfActiveThreads == 0) {
			statsUnlock();
			return;
		}

		int E = originalQueueSize - Q;
		int H = uniqueHosts;
		int D = numOfDnsLookups;
		int I = uniqueIps;
		int R = passedRobots;
		int C = crawledUrls;
		int L = totalLinks;
		statsUnlock();

		int timeElapsed = (int)(clock() - startTime) / (CLOCKS_PER_SEC / 1000);

		printf("Stat Thread:\n");
		printf("QueueSize: %f \n\n", Q);


		L /= 1000;

		printf("[%3d] %3d Q %6d E %7d H %6d D %6d I %5d R %5d C %5d L %4d K\n", timeElapsed, currNumOfActiveThreads, Q, E, H, D, I, R, C, L);
		

		

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

