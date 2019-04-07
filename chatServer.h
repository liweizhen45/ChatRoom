#pragma once

#include "unp.h"
#define MAXLINE 4096
using namespace std;

class chatServer{
public:
	chatServer();
	~chatServer();
	void connectClient();

private:
	int listenfd;
	int connfd[2];
	mutex m_mutex;
	chatServer(const chatServer&);
	chatServer& operator=(const chatServer&);
	
	bool ifquit;
	void readAndSend(int target);
	vector<thread*> Threads;

};




