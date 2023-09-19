#include "Crawler.h"


#define MAX_URL_LEN			2048

Crawler::Crawler(int numOfThreads) : originalQueueSize(0), uniqueHosts(0), numOfDnsLookups(0), uniqueIps(0), passedRobots(0), crawledUrls(0), totalLinks(0), status200(0), status300(0), status400(0), status500(0), statusOther(0) {
	this->maxThreadNum = numOfThreads;
	finishedThreads = 0;

	// initial state NOT locked
	mutexQueue = CreateMutex(NULL, 0, NULL);
	mutexStats = CreateMutex(NULL, 0, NULL);
	
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


	char link[MAX_URL_LEN];
	while (inputFile.getline(link, MAX_URL_LEN)) {
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

	printf("Origininal Queue Size %d\n", originalQueueSize);
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

	int waitTime = GetCurrentThreadId() / 10;
	
	// sync 
	while (!q.empty()) {
		queueLock();

		Url url;
		if (!q.empty()) {

			url = q.front();
			// printf("thread Run %d with wait time %d started running with %s\n", GetCurrentThreadId(), waitTime, q.front().baseUrl.c_str()); // print inside mutex to avoid garbage
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

			robotsAttempted += sock.robotAttempted;

			bytesDownloaded += sock.bytesDownloaded;

			totalLinks += sock.numOfLinks;

			int statusCode = sock.statusCode;
			statsUnlock();


			if (statusCode == -1) {}
			else if (statusCode < 200 || statusCode > 500) {
				statusOther++;
			}
			else if (statusCode < 300) {
				status200++;
			}
			else if (statusCode < 400) {
				status300++;
			}
			else if (statusCode < 500) {
				status400++;
			}
			else {
				status500++;
			}


		//	Sleep(waitTime); // FIXME: delete me

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
	clock_t startTimeBetweenStatsThreadWakeup = clock();

	while (true) {


		// FIXME: return back to 2000
		DWORD waitResult = WaitForSingleObject(eventQuit, 2000); // Wait for 3 seconds or until the event is signaled
		if (waitResult == WAIT_OBJECT_0) {
			break;
		}

		// lock mutex
		WaitForSingleObject(mutexQueue, INFINITE); // wait for mutex
		int Q = q.size();
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

		int tempBytesDownloaded = bytesDownloaded; // DELETE

		int timeElapsedSinceLastStatsThread = (int)(clock() - startTimeBetweenStatsThreadWakeup) / (CLOCKS_PER_SEC);


		float crawlingSpeed = C / 2000; // pages per second
		float downloadRate = (bytesDownloaded / 2000) * 8; // bytes to bits conversion

		statsUnlock();

		int timeElapsed = (int)(clock() - startTime) / (CLOCKS_PER_SEC);

		// FIXME`
		// L /= 1000;

		printf("[%3d] %3d Q %6d E %7d H %6d D %6d I %5d R %5d C %5d L %4d K\n", timeElapsed, currNumOfActiveThreads, Q, E, H, D, I, R, C, L);
		printf("\t*** crawling %.3g pps @ %.3g Mbps with total bytes downloaded %d over %d time\n", crawlingSpeed, downloadRate, tempBytesDownloaded, timeElapsedSinceLastStatsThread);
		

		startTimeBetweenStatsThreadWakeup = clock();
		

	}

}

void Crawler::quitStatsThread() {
	SetEvent(eventQuit);

}


void Crawler::printSummary() {

	printf("\n");
	printf("Extracted %i URLs @ %i/s\n", originalQueueSize, 0);
	printf("Looked up %i DNS names @ %i/s\n", numOfDnsLookups, 0);
	printf("Attempted %i site robots @ %i/s\n", robotsAttempted, 0);
	printf("Crawled %i pages @ %i/s (0.23MB)\n", crawledUrls, 0);
	printf("Parsed %i links @ %i/s\n", totalLinks, 0);
	printf("HTTP codes: 2xx = %i, 3xx = %i, 4xx = %i, 5xx = %i, other = %i\n", 5, status200, status300, status400, status500);
}

