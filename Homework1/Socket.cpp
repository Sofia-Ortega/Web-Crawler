
/*
*
* Name: Sofia Ortega
* Class: CSCE 463-500
* Semester: Fall 2023
*
*/


#include "Socket.h"



set<DWORD> Socket::seenIPs;
// set<char*, Socket::Comparator> Socket::seenHosts;
set<string> Socket::seenHosts;

Socket::Socket(char* link) : url(Url(link)) {

	readReady = false;

	uniqueHost = 0;
	successfulDNSNum = 0;
	uniqueIp = 0;
	robotAttempted = 0;
	passedRobots = 0;
	crawledUrlSuccess = 0;
	bytesDownloaded = 0;

	numOfLinks = 0;

	statusCode = -1;

	// initialize member variables
	this->buffer = new char[BUFFER_SIZE];
	this->capacity = BUFFER_SIZE;
	this->size = 0;

	// check if host duplicate
//	printf("\tChecking host uniqueness...");
	string host = url.host;
	auto seenHostResult = seenHosts.insert(host);
	if (seenHostResult.second == false) {
	//	printf("%s,%s,%i\n", url.baseUrl, url.host, uniqueHost);
		return;
		throw std::exception("failed - Duplicate Host");
	}
	uniqueHost = 1; // passed host uniqueness
	// printf("%s,%s,%i\n", url.baseUrl, url.host, uniqueHost);
//	printf("passed\n");

//	printf("\tDoing DNS... ");
	startClock();

	
	SOCKET roboSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (roboSock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		return;
		throw std::exception();
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() generated error %d\n", WSAGetLastError());
		return;
		throw std::exception();
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
			// throw std::exception("Invalid string: neither FQDN nor IP address \n");
			return;
		}


		// copy into sin_addr the first IP address
		memcpy((char*)&(server.sin_addr), remote->h_addr, remote->h_length);


	}
	else {
		// valid IP
		// directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;

	}

	successfulDNSNum++;

	server.sin_family = AF_INET;
	server.sin_port = htons(url.port);

//	printf("done in %i ms, found %s\n", endClock(), inet_ntoa(server.sin_addr));

	// check duplicate ip
//	printf("\tChecking IP uniqueness...");
	DWORD checkIP = server.sin_addr.s_addr;
	auto setResult = seenIPs.insert(checkIP);
	if (setResult.second == false) {
		return;
		throw std::exception("failed - Duplicate IP");
	}
	uniqueIp++;
//	printf("passed\n");


	// connect to robot page
//	printf("\tConnecting on robots... ");
	robotAttempted = 1;

	startClock();

	int connectResult = connect(roboSock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (connectResult == SOCKET_ERROR) {
		// printf("Connection error %d for url %s\n", WSAGetLastError(), url.baseUrl);
		throw std::exception();
		return;
	}
//	printf("done in %i ms\n", endClock());


	// read robot.txt
//	printf("\tLoading...");
	startClock();

	char* getRobotRequest = formatRobotRequest();
	int readResult = readRequestIntoBuffer(getRobotRequest, roboSock, 16384);
	if (readResult == -1) {
		throw std::exception();
		return;
	}

	delete[] getRobotRequest;

	//	printf("done in %i ms with %i bytes\n", endClock(), size);

	closesocket(roboSock);


	// check status header
//	printf("\tVerifying header... ");
	int roboStatusCode = getStatusCode();
//	printf("status code %i\n", statusCode);


	// check if robots.txt exists
	if (roboStatusCode >= 400 && roboStatusCode <= 499) {
		robotsNotAllowed = false;
		passedRobots++;
	}
	else {
		robotsNotAllowed = true;
		return;
	}



	// connect to socket on port 
//	printf("      * Connecting on page...");
	startClock();

	int connectResult2 = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
	if (connectResult2 == SOCKET_ERROR) {
		// printf("Connection error %d\n", WSAGetLastError());
		return;
		throw std::exception();
	}

	readReady = true;

//	printf("done in %i ms\n", endClock());

};

Socket::~Socket() {

	closesocket(sock);
	if(buffer) 
		delete[] buffer;

	url.deleteUrl();
}



char * Socket::formatGetRequest() {
	
	const char* a = "GET ";
	const char* b = url.request;
	const char* c = " HTTP/1.0\r\n";

	const char* d = "User-agent: SofiaSpyCrawler/1.1\r\n";
	const char* e = "Host: ";
	const char* f = url.host;
	const char* g = "\r\n";

	const char* h = "Connection: close\r\n\r\n";


	// total length of http request
	size_t getRequestLength = strlen(a) + strlen(b) + strlen(c) +
		strlen(d) + strlen(e) + strlen(f) + strlen(g) + strlen(h) + 1;


	char* getRequest = new char[getRequestLength];

	strcpy_s(getRequest, getRequestLength, a);
	strcat_s(getRequest, getRequestLength, b);
	strcat_s(getRequest, getRequestLength, c);
	strcat_s(getRequest, getRequestLength, d);
	strcat_s(getRequest, getRequestLength, e);
	strcat_s(getRequest, getRequestLength, f);
	strcat_s(getRequest, getRequestLength, g);
	strcat_s(getRequest, getRequestLength, h);

	getRequest[getRequestLength - 1] = '\0';

	return getRequest;

}

char* Socket::formatRobotRequest() {

	const char* a  = "HEAD /robots.txt HTTP/1.0\r\n";
	const char* b  = "User-agent: SofiaSpyCrawler/1.1\r\n";
	
	const char* c = "Host: ";
	const char* d = url.host;
	const char* e = "\r\n";

	const char* f = "Connection: close\r\n";
	const char* g = "\r\n";

	// total length of http request
	size_t getRequestLength = strlen(a) + strlen(b) + strlen(c) +
		strlen(d) + strlen(e) + strlen(f) + strlen(g) + 1;


	// allocate memory
	char* getRequest = new char[getRequestLength];

	// format getRequest
	strcpy_s(getRequest, getRequestLength, a);
	strcat_s(getRequest, getRequestLength, b);
	strcat_s(getRequest, getRequestLength, c);
	strcat_s(getRequest, getRequestLength, d);
	strcat_s(getRequest, getRequestLength, e);
	strcat_s(getRequest, getRequestLength, f);
	strcat_s(getRequest, getRequestLength, g);


	getRequest[getRequestLength - 1] = '\0';

	return getRequest;

}

int Socket::getStatusCode() {

	// ************* Verifying Header ****************

	char httpHeader[] = { 'H', 'T', 'T', 'P', '\0' };
	for (int i = 0; i < 4; i++) {
		if (buffer[i] != httpHeader[i]) {
			// printf("Non-Http Reply received - abort\n");
			return -1;
		}
	}

	char* numStr = new char[4];
	for (int i = 9; i < 12; i++) {
		numStr[i - 9] = buffer[i];
	}
	numStr[3] = '\0';


	int myStatusCode = atoi(numStr);

	return myStatusCode;
}

void Socket::resizeBuffer() {
	char* newBuffer = new char[capacity * 2];
	memcpy(newBuffer, buffer, size);

	delete[] buffer;
	buffer = newBuffer;

	capacity *= 2;
}

void Socket::clearBuffer() {
	size = 0;
	buffer[0] = '\0';
}

void Socket::startClock() {
	startTime = clock();
}

int Socket::endClock() {
	clock_t timeElapsed = clock() - startTime;
	return (int)timeElapsed / (CLOCKS_PER_SEC / 1000);
}

int Socket::readRequestIntoBuffer(char* getRequest, SOCKET mySock, int maxDownloadSize) {
	
	size_t sendResult = send(mySock, getRequest, (int)strlen(getRequest), 0);
	if (sendResult == SOCKET_ERROR) {
		// printf("Sending error: %d with url %s\n", WSAGetLastError(), url.baseUrl);
		return -1;
	}

	FD_SET readSet;


	timeval timeout;
	timeout.tv_sec = 3; // 10 seconds b4 it timesout
	timeout.tv_usec = 0;

	while (true) {

		FD_ZERO(&readSet);
		FD_SET(mySock, &readSet);


		int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
		if (selectResult) {

			int bytes = recv(mySock, buffer + size, BUFFER_SIZE - 1, 0);
			size += bytes;

			if (bytes > 0) {
				// printf("%s", buffer);
				// printf("\nbytes read: %i\n", bytes);
			}
			else if (bytes == 0) {
				// FIXME: make sure space for null terminated 
				buffer[size] = '\0';
				bytesDownloaded += size;
				size++;
				break;
			}
			else {
				printf("reading failed in recv() %d\n", WSAGetLastError());
				return -1;
			}


			// prepare buffer for next addition
			// doubles the capacity
			if (size + BUFFER_SIZE >= capacity) {
				if (capacity * 2 > maxDownloadSize) {
					printf("Download Size exceeded\n");
					return -1;
				}
				resizeBuffer();
			}

		}
		else if (selectResult == 0) {
			printf("timeout exceeded\n");
			return -1;
		}
		else {
			printf("Reading Socket error %i, %i\n", selectResult, WSAGetLastError());
			return -1;
		}

	}
}

void Socket::Read(void) {
	if (robotsNotAllowed) return;
	if (!readReady) return;

	clearBuffer();

	// printf("\tLoading... ");
	startClock();

	char* getRequest = formatGetRequest();
	int readResult = readRequestIntoBuffer(getRequest, sock, 2097152);
	if (readResult == -1) {
		return;
	}

	delete[] getRequest;


	// printf("done in %i ms with %i bytes\n", endClock(), size);


	// Status Code
	//	printf("\tVerifying header... ");
	int myStatusCode = getStatusCode();
	if (myStatusCode == -1) return;


	 // printf(" \t status code %i\n", statusCode);


	// ***************** Print Header ****************
	char endHeaderCharacters[] = "\r\n\r\n";
	char* endHeader = strstr(buffer, endHeaderCharacters);
	char* header;
	if (!endHeader) {
//		printf("---------------------------\n");
//		printf("%s", buffer);
		return;
	}
	else {
		*(endHeader - 1) = '\0';
		header = buffer;
		buffer = endHeader;
	}

	crawledUrlSuccess = 1;
	this->statusCode = myStatusCode;

	// ***************** Parse ********************
	if (myStatusCode >= 200 && myStatusCode <= 299) {
		Parse();
	}


	/*
	// ***************** Print Header ****************
	printf("---------------------------\n");
	printf("%s\n", header);

	// print buffer
	printf("%s\n", buffer);
	*/
	

	buffer = header;
}

void Socket::Parse(void) {
//	printf("      + Parsing page... ");
	startClock();

	HTMLParserBase* parser = new HTMLParserBase;

	int nLinks;
	char* baseUrl = url.getBaseUrl();
	char* linkBuffer = parser->Parse(buffer, size + 1, baseUrl, (int)strlen(baseUrl), &nLinks);

	if (nLinks < 0) {

//		printf("Parsing error\n");
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

//	printf("done in %i ms with %d links\n", endClock(), nLinks);

	numOfLinks = nLinks;

// 	printf("\n\n\n*****************************************\nCONSIDER THIS URL CRAWLED\n******************************\n\n");

	delete parser;

}

