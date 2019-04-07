#include "chatServer.h"

using namespace std;


chatServer::chatServer():ifquit(false){
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		cerr<<"fail to make a listenfd"<<endl;
		exit(1);
	}
	
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6666);

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		cerr<<"fail to bind socket"<<endl;
		exit(1);	
	}
	
	if(listen(listenfd,10) == -1){
		cerr<<"fail to listen"<<endl;
		exit(1);
	}
	cout<<"server initializtion complete"<<endl;
}


chatServer::~chatServer(){
	for(auto it = Threads.begin();it!=Threads.end();it++){
		if((*it)->joinable()){
			(*it)->join();
		}
		delete *it;
	}
	Threads.clear();
	close(connfd[0]);
	close(connfd[1]);
	close(listenfd);
	cout<<"quit success ,thx for use"<<endl;
}

void chatServer::connectClient(){
	cout<<"waiting for connection..."<<endl;
	if((connfd[0] = accept(listenfd,(struct sockaddr*)NULL,NULL)) == -1){
		cerr<<"cant connect client1"<<endl;
		exit(0);
	}
	cerr<<"client1 connected"<<endl;
	if((connfd[1] = accept(listenfd,(struct sockaddr*)NULL,NULL)) == -1){
		cerr<<"cant connect client2"<<endl;
		exit(0);
	}
	cerr<<"client2 connected"<<endl;
	Threads.push_back(new thread(bind(&chatServer::readAndSend,this,0)));
	Threads.push_back(new thread(bind(&chatServer::readAndSend,this,1)));
	
}

void chatServer::readAndSend(int target){
	while(!ifquit){
		char targetNumber[MAXLINE];
		char recvMsg[MAXLINE];	
		
		if(target<0 || target>1) {
			cerr<<"wrong target!"<<endl;
			exit(0);			
		}
		
		int n = recv(connfd[target],recvMsg,MAXLINE,0);
		recvMsg[n] = '\0';
		cout<<"recving msg "<<recvMsg<<" from client "<<target<<endl;
		
		if(strcmp(recvMsg,"quit") == 0) {
			unique_lock<mutex> lk(m_mutex);			
			ifquit = true;
			char *quitInfo = "the other one has quitted.";
			if(send(connfd[1-target],quitInfo,strlen(quitInfo),0) < 0){
				cerr<<"sending msg error"<<endl;
			}
			continue;
		}

		if(send(connfd[1-target],recvMsg,strlen(recvMsg),0) < 0){
			cerr<<"sending msg error"<<endl;
		}

		
	}
	cout<<"client "<<target<<" quit chatting : "<<endl;
}



