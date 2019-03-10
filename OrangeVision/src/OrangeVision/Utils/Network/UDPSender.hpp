#pragma once
#ifndef UDP_SENDER_HPP
#define UDP_SENDER_HPP


#ifdef _WIN32

#elif __linux__
#include <netinet/in.h>

class UDPSender {
public:
	UDPSender(const char* ip, int port);
	UDPSender(const char* ip, const char* port);
	~UDPSender();
	bool send(const void *data, int len);
	bool setTarget(const char* ip, int port);
	bool setTarget(const char* ip, const char* port);
private:
	int sockfd;
	struct addrinfo hints { 0 }, *results;
};
#endif
#endif