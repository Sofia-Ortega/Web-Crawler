
/*

Name: Sofia Ortega
Class: CSCE 463-500
Semester: Fall 2023

*/

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {


	if (argc == 3 && atoi(argv[1]) != 1) {
		printf("[ERROR] Incorrect input");
		return -1;
	} 

	string inputFileName = argv[2];


	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}



	// ------------ From command line -------------
	
	/*
	string link = argv[1];
	try {
		Url url(link);
		Socket sock(url);
		sock.Read();
	}
	catch (const std::exception& e) {
		printf("[ERROR] %s", e.what());
	}

	*/


	// ------------ From file ------------------
	std::ifstream inputFile(inputFileName);

	if (!inputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return -1;
	}

	string link;
	while (std::getline(inputFile, link)) {
		try {
			Url url(link);




			Socket sock(url);
			sock.Read();
		}
		catch (const std::exception& e) {
			printf("[ERROR] %s", e.what());
		}
	}

	
	// cleanup!
	WSACleanup();
	







}