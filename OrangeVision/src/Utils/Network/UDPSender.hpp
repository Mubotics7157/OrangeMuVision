#pragma once
#ifndef UDP_SENDER_HPP
#define UDP_SENDER_HPP


#ifdef _WIN32

#elif __linux__
class UDPSender {
public:
	UDPSender(const char* ip, int port);
	~UDPSender();
	bool Send(const void *data, int len);
private:
	int sockfd;
};
#endif
#endif