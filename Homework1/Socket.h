#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

#include "Url.h"
#include "HTMLParserBase.h"

using std::string;

class Socket
{
private:
	const int BUFFER_SIZE = 1024;

	char* buffer;
	int size;
	int capacity;

	Url url;

	SOCKET sock;

	string formatGetRequest();
	void resizeBuffer();

public:
	Socket(const Url& urlInput);
	~Socket();
	void Read(void);
	void Parse(void);

};

