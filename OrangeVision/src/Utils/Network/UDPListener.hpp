#pragma once
#ifndef UDP_LISTENER_HPP
#define UDP_LISTENER_HPP

#ifdef _WIN32

#elif __linux__
class UDPListener {
public:
	UDPListener(int port);
	~UDPListener();
	bool Recv(void *data, int len);
private:
	int sockfd;
};
#endif
#endif