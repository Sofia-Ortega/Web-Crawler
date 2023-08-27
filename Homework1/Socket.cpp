#include "Socket.h"


Socket::Socket(char* address) {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	url = address;

	// connectin to server
	struct hostent* remote;

	// structure to connect to server
	struct sockaddr_in server;

	// assume ip is in IP address
	DWORD IP = inet_addr(address);
	if (IP == INADDR_NONE) {
		printf("inaddr none");
		// if not valid ip, do DNS Lookup
		remote = gethostbyname(address);
		if (remote == NULL) {
			printf("Invalid string: neither FQDN nor IP address \n");
			return;
		}

		// copy into sin_addr the first IP address
		memcpy((char*)&(server.sin_addr), remote->h_addr_list, remote->h_length);
	}
	else {
		// valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	printf("server");
	// connect to socket on port 80
	int connectResult = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (connectResult == SOCKET_ERROR) {
		printf("Connection error %d\n", WSAGetLastError());
		return;
	}

	printf("Socket succesfully connected + setup!");
};

Socket::~Socket() {
	closesocket(sock);
}

string Socket::formatGetRequest(string host) {
	string s;
	s  = "GET / HTTP/1.0\r\n";
	s += "User-agent: myTAMUcrawler/1.0\r\n";
	s += "nHost: " + host + "\r\n";
	s += "Connection: close\r\n";
	s += "\r\n";

	return s;

}


void Socket::Read(void) {

	const string getRequest = formatGetRequest(url);
	
	int sendResult = send(sock, getRequest.c_str(), getRequest.size(), 0);
	if (sendResult == SOCKET_ERROR) {
		printf("Sending error: %d\n", WSAGetLastError());
		return;
	}

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	int bytesRead = 1;


	FD_SET readSet;


	timeval timeout;
	timeout.tv_sec = 10; // 10 seconds b4 it timesout
	timeout.tv_usec = 0;

	while (true) {

		memset(buffer, 0, BUFFER_SIZE - 1);

		FD_ZERO(&readSet);
		FD_SET(sock, &readSet);


		int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
		if (selectResult) {
			int bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
			if (bytesRead > 0) {
				printf("%s", buffer);
			}
			else if (bytes == 0) {
				printf("connection closed\n");
				return;
			}
			else {
				printf("reading failed\n");
				return;
			}
		}
		else if (selectResult == 0) {
			printf("timeout exceeded\n");
			return;
		}
		else {
			printf("Reading Socket error %d\n", WSAGetLastError());
		}
	}







}