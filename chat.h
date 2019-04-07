#pragma once

#include <cstring>
#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <mutex>
#include <chrono>

#define MAXLINE 4096

using namespace std;

class chat{
public:
	chat();
	void beginChat();
	void endChat();

private:
	bool ifquit;
	int sockfd;	
	chat(const chat&);
	chat& operator=(const chat&);

	void recvMsg();
	void sendMsg();
	mutex m_mutex;
	vector<thread*> Threads; 
};


