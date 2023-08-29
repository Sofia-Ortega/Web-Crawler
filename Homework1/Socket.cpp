#include "Socket.h"


Socket::Socket(const Url& urlInput) {

	// initialize member variables
	this->buffer = new char[BUFFER_SIZE];
	this->capacity = BUFFER_SIZE;
	this->size = 0;

	this->url = Url(urlInput);

	printf("\tDoing DNS... ");
	startClock();

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		throw std::exception();
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
			throw std::exception("Invalid string: neither FQDN nor IP address \n");
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

	printf("done in %i ms, found %s\n", endClock(), inet_ntoa(server.sin_addr));



	// connect to socket on port 80
	printf("\t* Connecting on page...");
	startClock();

	int connectResult = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (connectResult == SOCKET_ERROR) {
		printf("Connection error %d\n", WSAGetLastError());
		throw std::exception();
	}

	printf("done in %i ms\n", endClock());

};

Socket::~Socket() {
	closesocket(sock);
}

string Socket::formatGetRequest() {
	string s;

	s  = "GET " + url.request + " HTTP/1.0\r\n";
	s += "User-agent: SofiaSpyCrawler/1.1\r\n";
	s += "Host: www." + url.host + "\r\n";
	s += "Connection: close\r\n";
	s += "\r\n";

	printf("\n\nGet request \n%s\n\n", s.c_str());

	return s;

}

void Socket::resizeBuffer() {
	char* newBuffer = new char[capacity * 2];
	memcpy(newBuffer, buffer, size);

	delete[] buffer;
	buffer = newBuffer;

	capacity *= 2;
}

void Socket::startClock() {
	startTime = clock();
}

int Socket::endClock() {
	clock_t timeElapsed = clock() - startTime;
	return (int)timeElapsed / (CLOCKS_PER_SEC / 1000);
}

void Socket::Read(void) {

	printf("\tLoading... ");
	startClock();

	const string getRequest = formatGetRequest();

	size_t sendResult = send(sock, getRequest.c_str(), getRequest.size(), 0);
	if (sendResult == SOCKET_ERROR) {
		printf("Sending error: %d\n", WSAGetLastError());
		return;
	}

	FD_SET readSet;


	timeval timeout;
	timeout.tv_sec = 2; // 10 seconds b4 it timesout
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
				printf("reading failed in recv() %d\n", WSAGetLastError());
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
			printf("Reading Socket error %d\n", selectResult, WSAGetLastError());
		}

	}

	printf("done in %i ms with %i bytes\n", endClock(), size);


	// ************* Verifying Header ****************
	printf("\tVerifying header... ");

	char httpHeader[] = { 'H', 'T', 'T', 'P', '\0' };
	for (int i = 0; i < 4; i++) {
		if (buffer[i] != httpHeader[i]) {
			printf("Non-Http Reply received - abort\n");
			return;
		}
	}

	char* numStr = new char[4];
	for (int i = 9; i < 12; i++) {
		numStr[i - 9] = buffer[i];
	}
	numStr[3] = '\0';


	int statusCode = atoi(numStr);

	printf(" status code %i\n", statusCode);

	if (statusCode == 200) {
		printf("++++++++++++++++++++++++++++++++++++++++++++ 200 ++++++++++++++++++++++++++++++++++");
	}
	else if (statusCode == 400) {
		printf("}||||||||||||||||||||||||||||||||||||||||| 400 |||||||||||||||||||||||||||||||||||||||||||||||||");
	}


	// ***************** Print Header ****************
	char* angleBracket = strchr(buffer, '<');
	char* header;
	if (!angleBracket) {
		printf("%s", buffer);
		return;
	}
	else {
		*(angleBracket - 1) = '\0';
		header = buffer;
		buffer = angleBracket;
	}

	// ***************** Parse ********************
	if (statusCode == 200) {
		Parse();
	}


	// ***************** Print Header ****************
	printf("---------------------------\n");
	printf("%s\n", header);




	//printf("%s\n", buffer);
}

void Socket::Parse(void) {
	printf("\t+ Parsing page... ");
	startClock();

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

	printf("done in %i ms with %d links\n", endClock(), nLinks);

}

