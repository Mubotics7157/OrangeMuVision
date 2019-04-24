#include "UDPListener.hpp"

#ifdef _WIN32

#elif __linux__
#include "UDPListener.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

UDPListener::UDPListener(int port) : sockfd(socket(PF_INET, SOCK_DGRAM, 0)) {
	sockaddr_in addr{ 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		close(sockfd);
		//NOT BINDED
	}
}

UDPListener::~UDPListener() {
	close(sockfd);
}

bool UDPListener::Recv(void* data, int len) {
	int received = 0;
	while (len > 0) {
		received = recv(sockfd, (unsigned char*)data + received, len, 0);
		if (received == -1) {
			return false;
		}
		data += received;
		len -= received;
	}
	return true;
}
#endif