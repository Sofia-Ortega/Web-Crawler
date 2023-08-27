#include "Socket.h"


Socket::Socket(char* address) {

	// initialize member variables
	this->buffer = new char[BUFFER_SIZE];
	this->capacity = BUFFER_SIZE;
	this->size = 0;

	this->url = address;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

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

	printf("Socket succesfully connected + setup!\n");
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

	FD_SET readSet;


	timeval timeout;
	timeout.tv_sec = 10; // 10 seconds b4 it timesout
	timeout.tv_usec = 0;

	while (true) {

		FD_ZERO(&readSet);
		FD_SET(sock, &readSet);


		int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
		if (selectResult) {

			int bytes = recv(sock, buffer + size, BUFFER_SIZE - 1, 0);
			size += bytes;

			if (bytes > 0) {
				// printf("%s", buffer);
				printf("%i\n", bytes);
			}
			else if (bytes == 0) {
				printf("connection closed\n");
				// FIXME: make sure space for null terminated 
				buffer[size] = '\0';
				printf("%s", buffer);
				return;
			}
			else {
				printf("reading failed\n");
				return;
			}


			// prepare buffer for next addition
			// double the capacity
			if (size + BUFFER_SIZE >= capacity) {
				char* newBuffer = new char[capacity * 2];
				memcpy(newBuffer, buffer, size);

				delete[] buffer;
				buffer = newBuffer;

				capacity *= 2;

			}
			 
		}
		else if (selectResult == 0) {
			printf("timeout exceeded\n");
			return;
		}
		else {
			printf("Reading Socket error %d\n", WSAGetLastError());
		}


		printf("---------------------------------\n");
		printf("final buffer contetns:\n");
		printf("capacity: %i\n", capacity);
		printf("size: %i\n", size);
		printf("---------------------------------\n");
		
	}







}