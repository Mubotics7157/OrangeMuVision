#include "UDPSender.hpp"


#ifdef _WIN32

#elif __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

UDPSender::UDPSender(const char* ip, int port) : sockfd(socket(PF_INET, SOCK_DGRAM, 0)) {
	sockaddr_in addr{ 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);
	if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		close(sockfd);
		//NOT CONNECTED
	}
}

UDPSender::~UDPSender() {
	close(sockfd);
}

bool UDPSender::Send(const void* data, int len) {
	int sent = 0;
	while (len > 0) {
		sent = send(sockfd, (unsigned char*)data, len, 0);
		if (sent == -1) {
			return false;
		}
		data += sent;
		len += sent;
	}
	return true;
}
#endif