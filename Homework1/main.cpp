

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::string;

int main() {
	cout << "Howdy World" << endl;

	string link = "http://tamu.edu:8080/cs/in:dex.php?test=1#something";
	string link2 = "http://google.com";
	string link3 = "http://128.194.135.72";
	string link4 = "http://www.dmoz.org";
	string link5 = "http://www.yahoo.com";
	string link6 = "http://s22.irl.cs.tamu.edu:990/view?test=1 ";


	// bad links
	string badlink1 = "http://xyz.com:0";
	string badlink2 = "http://xyz.com:/";
	string badlink3 = "ftp://yahoo.com";
	
	string links[] = { link, link2, link3, link4, link5, link6, badlink1, badlink2, badlink3};


	/*
	for (auto l : links) {
		try {
			Url url(l);
		}
		catch (const std::invalid_argument& e) {
			printf("%s\n", e.what());
		}
	}

	*/
/*

	// *************** Setup **********************
	WSADATA wsadata;

	// initialize winsocket
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// ************** Sockets ********************
	string here = "http://www.tamu.edu";
	string here2 = "http://128.194.135.72";
	Url url(here);
	Socket mySock(url);
	// mySock.Read();


	// *************** Clean up! *******************
	WSACleanup();

*/


	// scheme://host[:port][/path][?query][#fragment] 

	// char str[] = "128.194.135.72";
	// char str[] = "www.tamu.edu";
	//char str[] = "www.yahoo.com";
	// char str[] = "https://irl.cse.tamu.edu/";
	// char str[] = "irl.cse.tamu.edu";


	WSADATA wsadata;

	// initialize winsocket
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	
	string str = "http://www.dmoz.org";
	Url urlHere(str);


	Socket urlSock(urlHere);
	urlSock.Read();

	string str2 = "http://google.com";
	Url urlStr2(str2);
	Socket urlSock2(urlStr2);
	urlSock2.Read();


	// cleanup!
	WSACleanup();


	/*
	char filename[] = "tamu2018.html";

	// open html file
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	// process errors
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with %d\n", GetLastError());
		return 0;
	}

	// get file size
	LARGE_INTEGER li;
	BOOL bRet = GetFileSizeEx(hFile, &li);
	// process errors
	if (bRet == 0)
	{
		printf("GetFileSizeEx error %d\n", GetLastError());
		return 0;
	}

	// read file into a buffer
	int fileSize = (DWORD)li.QuadPart;			// assumes file size is below 2GB; otherwise, an __int64 is needed
	DWORD bytesRead;
	// allocate buffer
	char* fileBuf = new char[fileSize];
	// read into the buffer
	bRet = ReadFile(hFile, fileBuf, fileSize, &bytesRead, NULL);
	// process errors
	if (bRet == 0 || bytesRead != fileSize)
	{
		printf("ReadFile failed with %d\n", GetLastError());
		return 0;
	}

	// done with the file
	CloseHandle(hFile);

	// create new parser object
	HTMLParserBase* parser = new HTMLParserBase;

	char baseUrl[] = "http://www.tamu.edu";		// where this page came from; needed for construction of relative links

	int nLinks;
	char* linkBuffer = parser->Parse(fileBuf, fileSize, baseUrl, (int)strlen(baseUrl), &nLinks);

	// check for errors indicated by negative values
	if (nLinks < 0)
		nLinks = 0;

	printf("Found %d links:\n", nLinks);

	// print each URL; these are NULL-separated C strings
	for (int i = 0; i < nLinks; i++)
	{
		printf("%s\n", linkBuffer);
		linkBuffer += strlen(linkBuffer) + 1;
	}

	delete parser;		// this internally deletes linkBuffer
	delete fileBuf;

	return 0;
	*/






}