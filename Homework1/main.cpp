

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;

int main() {
	cout << "Howdy World" << endl;

	WSADATA wsaData;

	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	std::ifstream inputFile("input.txt");

	if (!inputFile.is_open()) {
		printf("[ERROR] Can't open file\n");
		return -1;
	}

	string link;
	while (std::getline(inputFile, link)) {
		try {
			//link = "http://www.symantec.com/verisign/ssl-certificates";
		 	//link = "http://128.194.135.72";
			// link = "http://aburningpatience.blogspot.com/2012_01_01_archive.html";
			// link = "http://www.symantec.com/verisign/ssl-certificates";
			// link = "http://360.ch/blog/magazine/2014/08/cadeaux-fiscaux-aux-guerisseurs-de-gays/";
			//link = "http://youtube.com/user/USIMLS";

			Url url(link);
			Socket sock(url);
			sock.Read();
			//break;
		}
		catch (const std::exception& e) {
			printf("[ERROR] %s", e.what());
		}
	}

	
	// cleanup!
	WSACleanup();
	







}