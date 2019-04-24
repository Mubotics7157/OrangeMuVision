#include "UDPSender.hpp"

#ifdef _WIN32

#elif __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

UDPSender::UDPSender(const char* ip, const char* port) {
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	setTarget(ip, port);
}

UDPSender::~UDPSender() {
	close(sockfd);
	freeaddrinfo(results);
}

bool UDPSender::send(const void* data, int len) {
	int sent = 0;
	while (len > 0) {
		sent = sendto(sockfd, (unsigned char*)data, len, 0, results->ai_addr, results->ai_addrlen);
		if (sent == -1) {
			return false;
		}
		data += sent;
		len += sent;
	}
	return true;
}

void UDPSender::setTarget(const char* ip, const char* port) {
	getaddrinfo(ip, port, &hints, &results);
	sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
}
#endif