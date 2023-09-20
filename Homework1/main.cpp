
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
	string tempLink = "http://361107.spreadshirt.net/en/GB/Shop/Index/index";
	try {
		Url url(tempLink);


	}
	catch (const std::exception& e) {
		printf("[ERROR] %s\n", e.what());
	}

	return 0;
	*/

	// -----------------------------------------------------------------------------
	/*
	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}


	string inputFileName = "URL-input-100.txt";
	std::ifstream myinputFile(inputFileName);
	
	if (!myinputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return -1;
	}

	myinputFile.seekg(0, myinputFile.end);
	int fileSize = myinputFile.tellg();

	printf("Opened %s with size %i\n\n", inputFileName.c_str(), fileSize);

	myinputFile.seekg(0, myinputFile.beg);

	string line;
	int robotsAttempted = 0;
	while (std::getline(myinputFile, line)) {

		try {

			char* link = new char[line.length() + 1];
			memset(link, 0, line.length() + 1);

			strcpy_s(link, line.length() + 1, line.c_str());


			Socket sock(link);
			sock.Read();


			


		}
		catch (const std::exception& e) {
			if(e.what() != "Unknown exception")
				printf("[ERROR] %s\n", e.what());
		}
	}

	cout << endl << "Set:" << endl;
	for (auto it = Socket::seenHosts.begin(); it != Socket::seenHosts.end(); it++) {
		cout << *it << endl;
	}



	
	// cleanup!
	WSACleanup();
	myinputFile.close();



	return 0;

	*/
	/*
	WSADATA wsaData;


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


	int numThreads = 10;
	// const char* inputFile = "URL-input-1M.txt";
	const char* inputFile = "URL-input-100.txt";

	bool runStatsThread = true;

	HANDLE* handles = new HANDLE[numThreads + 1];
	Crawler crawler = Crawler(numThreads);

	// read file + populate queue
	crawler.ReadFile(inputFile);

	// start stats thread
	if(runStatsThread) handles[numThreads] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startCrawlerStats, &crawler, 0, NULL);

	for (int i = 0; i < numThreads; i++) {
		handles[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startCrawlerRun, &crawler, 0, NULL);
	}

	for (int i = 0; i < numThreads; i++) {
		WaitForSingleObject(handles[i], INFINITE);
		CloseHandle(handles[i]);
	}

	if (runStatsThread) {
		crawler.quitStatsThread();

		WaitForSingleObject(handles[numThreads], INFINITE);
		CloseHandle(handles[numThreads]);
	}


	crawler.printSummary();

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