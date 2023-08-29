

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
	
	// string links[] = { link, link2, link3, link4, link5, link6, badlink1, badlink2, badlink3};
	string links[] = { link3 };

	WSADATA wsadata;

	// initialize winsocket
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsadata) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	
	for (auto l : links) {
		try {
			Url url(l);
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