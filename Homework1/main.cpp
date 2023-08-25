#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;

int main() {
	cout << "Howdy World" << endl;

	// scheme://host[:port][/path][?query][#fragment] 
	
	//char str[] = "128.194.135.72";
	//char str[] = "www.tamu.edu";
	//char str[] = "www.yahoo.com";
	// char link[] = "https://irl.cse.tamu.edu/";
	char str[] = "irl.cse.tamu.edu";


	WSADATA wsadata;

	// initialize winsocket
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Above main()
	// Below object

	// open TCP socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// structure used in DNS lookup
	struct hostent* remote;
	
	// structure to connect to server
	struct sockaddr_in server;

	// assume ip is an IP address
	DWORD IP = inet_addr(str);
	if (IP == INADDR_NONE) {

		// if not valid IP, do DNS lookup
		remote = gethostbyname(str);

		if (remote == NULL) {
			printf("Invalid string: neither FQDN nor IP address \n");
			return -1;
		}

		// copy into sin_addr the first IP address
		memcpy((char*)&(server.sin_addr), remote->h_addr, remote->h_length);

	}
	else {
		// if valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;

	}




	// setup port number and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80); // host-to-network flips byte order


	// connect to socket on port 80
	if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
		printf("Connection error: %d\n", WSAGetLastError());
		return -1;
	}


	cout << "connected!" << endl;


	// send http request here
	cout << "URL: " << str << endl << endl;
	const string getRequest = "GET / HTTP/1.0\r\nUser-agent: myTAMUcrawler/1.0\r\nHost: irl.cse.tamu.edu \r\nConnection: close\r\n\r\n";
	int sendResult = send(sock, getRequest.c_str(), getRequest.size(), 0);

	if (sendResult == SOCKET_ERROR) {
		printf("Sending error: %d\n", WSAGetLastError());
		return -1;
	}


	// receive request response here

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	int bytesRead = 1;

	while (bytesRead) {
		memset(buffer, 0, BUFFER_SIZE - 1);

		bytesRead = recv(sock, buffer, BUFFER_SIZE - 1, 0);

		if (bytesRead < 0) {
			printf("Reading FAILED");
			return -1;
		}
		else if (bytesRead == 0) {
			printf("connection closed");
			return -1;
		}
		else {
			printf("%s", buffer);
		}
	}
	
	



	// close socket
	closesocket(sock);

	// cleanup!
	WSACleanup();









}