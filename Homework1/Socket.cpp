#include "Socket.h"


Socket::Socket(const Url& urlInput) {

	// initialize member variables
	this->buffer = new char[BUFFER_SIZE];
	this->capacity = BUFFER_SIZE;
	this->size = 0;

	this->url = Url(urlInput);

	printf("\tDoing DNS... ");

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
	char* address = url.getAddress();

	DWORD IP = inet_addr(address);
	if (IP == INADDR_NONE) {
		// if not valid ip, do DNS Lookup
		remote = gethostbyname(address);
		if (remote == NULL) {
			printf("Invalid string: neither FQDN nor IP address \n");
			return;
		}

		// copy into sin_addr the first IP address
		memcpy((char*)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else {
		// valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(url.port);

	printf("found %s\n", inet_ntoa(server.sin_addr));



	// connect to socket on port 80
	printf("\t* Connecting on page...");
	int connectResult = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (connectResult == SOCKET_ERROR) {
		printf("Connection error %d\n", WSAGetLastError());
		return;
	}

	printf("done in %i ms\n", 0);


};

Socket::~Socket() {
	closesocket(sock);
}

string Socket::formatGetRequest() {
	string s;
	s  = "GET / HTTP/1.0\r\n";
	s += "User-agent: SofiaSpyCrawler/1.1\r\n";
	s += "nHost: " + url.host + "\r\n";
	s += "Connection: close\r\n";
	s += "\r\n";

	return s;

}

void Socket::resizeBuffer() {
	char* newBuffer = new char[capacity * 2];
	memcpy(newBuffer, buffer, size);

	delete[] buffer;
	buffer = newBuffer;

	capacity *= 2;
}

void Socket::Read(void) {

	printf("\tLoading... ");

	const string getRequest = formatGetRequest();
	
	size_t sendResult = send(sock, getRequest.c_str(), getRequest.size(), 0);
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
				// printf("\nbytes read: %i\n", bytes);
			}
			else if (bytes == 0) {
				// FIXME: make sure space for null terminated 
				buffer[size] = '\0';
				size++;
				break;
			}
			else {
				printf("reading failed\n");
				return;
			}


			// prepare buffer for next addition
			// doubles the capacity
			if (size + BUFFER_SIZE >= capacity) {
				resizeBuffer();
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

	printf("done in %i with %i bytes\n", 0, size);


	// ************* Verifying Header ****************
	printf("\tVerifying header... ");
	char* numStr = new char[4];
	for (int i = 9; i < 12; i++) {
		numStr[i - 9] = buffer[i];
	}
	numStr[3] = '\0';

	int statusCode = atoi(numStr);

	printf(" status code %i\n", statusCode);




	// ***************** Parse ********************
	if (statusCode == 200) {
		Parse();
	}


	// ***************** Print Header ****************
	char* angleBracket = strchr(buffer, '<');
	*angleBracket = '\0';





	printf("---------------------------\n");
	printf("%s\n", buffer);
}

void Socket::Parse(void) {
	printf("\t + Parsing page... ");
	HTMLParserBase* parser = new HTMLParserBase;

	int nLinks;
	char* baseUrl = url.getBaseUrl();
	char* linkBuffer = parser->Parse(buffer, size + 1, baseUrl, (int)strlen(baseUrl), &nLinks);

	if (nLinks < 0) {

		printf("Parsing error\n");
		nLinks = 0;
	}

	/*
	printf("Found %d links:\n", nLinks);

	for (int i = 0; i < nLinks; i++) {
		printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}

	delete parser;
	*/

	printf("done in %i ms with %d links\n", 0, nLinks);

}

