#pragma once
#ifndef UDP_SENDER_HPP
#define UDP_SENDER_HPP


#ifdef _WIN32

#elif __linux__
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class UDPSender {
public:
	UDPSender(const char* ip, const char* port);
	~UDPSender();
	bool send(const void *data, int len);
	void setTarget(const char* ip, const char* port);
private:
	int sockfd;
	addrinfo hints {}, *results;
};
#endif
#endif