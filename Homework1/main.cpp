
/*
*
* Name: Sofia Ortega
* Class: CSCE 463-500
* Semester: Fall 2023
*
*/

#include "stdafx.h"
#include "Crawler.h"

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;


class Test {
private:
	HANDLE mutex;
	HANDLE finished;
	HANDLE eventQuit;
public:
	Test() {
		// mutex to access critical section; initial state NOT locked
		mutex = CreateMutex(NULL, 0, NULL);
		// create a semaphore; min: 0, max: 2; counts number of active threads
		finished = CreateSemaphore(NULL, 0, 2, NULL);
		// create a quit event; manual reset, inital state = not signaled
		eventQuit = CreateEvent(NULL, true, false, NULL);
	}

	void threadA() {

		// mutex
		WaitForSingleObject(mutex, INFINITE); // wait for mutex
		printf("thread A %d started\n", GetCurrentThreadId()); // print inside mutex to avoid garbage
		Sleep(1000); // critical section
		ReleaseMutex(mutex); // release mutex


		// signal that thread has finihsed job
		ReleaseSemaphore(finished, 1, NULL);

		// wait for threadB to exit
		WaitForSingleObject(eventQuit, INFINITE);

		// print about to exit
		WaitForSingleObject(mutex, INFINITE);
		printf("thread A %d quitting on event\n", GetCurrentThreadId());
		ReleaseMutex(mutex);

	}

	void threadB() {
		// wait for both thread As to finish
		WaitForSingleObject(finished, INFINITE);
		WaitForSingleObject(finished, INFINITE);

		// no need to sync bc only thread B is left
		printf("thread B is running!\n");
		Sleep(3000);

		printf("thread B setting up quit event\n");

		// force other threads to quit
		SetEvent(eventQuit);



	}
};

UINT startThreadA(LPVOID pParam) {
	Test* p = (Test*)pParam;
	p->threadA();
	return 0;
}

UINT startThreadB(LPVOID pParam) {
	Test* p = (Test*)pParam;
	p->threadB();
	return 0;
}

UINT startCrawlerRun(LPVOID pParam) {
	Crawler* c = (Crawler*)pParam;
	c->Run();
	return 0;
}

UINT startCrawlerStats(LPVOID pParam) {
	Crawler* c = (Crawler*)pParam;
	c->printStats();
	return 0;
}

int main(int argc, char* argv[]) {

	/*
	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	string link = "http://allthebuildingsinnewyork.com/";
	try {
		Url url(link);
		Socket sock(url);
		sock.Read();
	}
	catch (const std::exception& e) {
		printf("[ERROR] %s", e.what());
	}

	WSACleanup();
	return 1;

	*/
	// ----------------------------------------------------


	int numThreads = 3;
	string inputFile = "input.txt";

	HANDLE* handles = new HANDLE[numThreads + 1];
	Crawler crawler = Crawler(numThreads);

	// read file + populate queue
	crawler.ReadFile(inputFile);

	// start stats thread
	handles[numThreads] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startCrawlerStats, &crawler, 0, NULL);

	for (int i = 0; i < numThreads; i++) {
		handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startCrawlerRun, &crawler, 0, NULL);
	}

	// handles[numThreads]; // stat thread

	for (int i = 0; i < numThreads; i++) {
		WaitForSingleObject(handles[i], INFINITE);
		CloseHandle(handles[i]);
	}

	
	printf("terminating stats thread\n");
	crawler.quitStatsThread();

	WaitForSingleObject(handles[numThreads], INFINITE);
	CloseHandle(handles[numThreads]);

	crawler.printSummary();
	printf("terminating main()\n");

	return 0;





	// --------------------------------------------------------------------------------------------------------

	HANDLE* handles2 = new HANDLE[3];
	Test p;

	// structure p is the shared space between threads
	handles2[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startThreadA, &p, 0, NULL);
	handles2[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startThreadA, &p, 0, NULL);
	handles2[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startThreadB, &p, 0, NULL);

	for (int i = 0; i < 3; i++) {
		WaitForSingleObject(handles2 [i], INFINITE);
		CloseHandle(handles2 [i]);
	}

	printf("terminating main()\n");
	
	return 0;

	/*

	if (argc == 1) {
		printf("[ERROR] Enter input");
		return -1;
	}

	if (argc == 3 && atoi(argv[1]) != 1) {
		printf("[ERROR] Incorrect input");
		return -1;
	} 

	bool threading = true;
	if (argc == 2) {
		threading = false;
	}
	


	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}



	// ------------ From command line -------------
	
	if (!threading) {
		string link = argv[1];
		try {
			Url url(link);
			Socket sock(url);
			sock.Read();
		}
		catch (const std::exception& e) {
			printf("[ERROR] %s", e.what());
		}

		return 1;
	}


	// ------------ From file ------------------
	string inputFileName = argv[2];
	std::ifstream inputFile(inputFileName);
	
	if (!inputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return -1;
	}

	inputFile.seekg(0, inputFile.end);
	int fileSize = inputFile.tellg();

	printf("Opened %s with size %i", inputFileName.c_str(), fileSize);

	inputFile.seekg(0, inputFile.beg);

	string link;
	while (std::getline(inputFile, link)) {
		try {
			Url url(link);

			// make url robot.txt
			// Socket sock  -> connect to robot.url
			// get status code

			Socket sock(url);
			sock.Read();
		}
		catch (const std::exception& e) {
			if(e.what() != "Unknown exception")
				printf("[ERROR] %s", e.what());
		}
	}

	
	// cleanup!
	WSACleanup();
	inputFile.close();
	
	*/







}