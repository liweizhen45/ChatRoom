#include "chat.h"
#include "unp.h"

using namespace std;

chat::chat():ifquit(false){
	cout<<"connect server ...."<<endl;
	char *port = "127.0.0.1";
	struct sockaddr_in servaddr;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		cerr<<"create socket failed"<<endl;
		exit(1);	
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6666);
	
	if(inet_pton(AF_INET,port,&servaddr.sin_addr) == -1){
		cerr<<"inet_pton fail"<<endl;
		exit(1);	
	}

	if(connect(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		cerr<<"connect error"<<endl;
		exit(1);
	}
	cout<<"server connected!"<<endl;
}



void chat::endChat(){
	while(!ifquit);
	shutdown(sockfd,SHUT_RDWR);
	for(auto it=Threads.begin(); it!=Threads.end() ; it++){
		if((*it)->joinable())
			(*it)->join();
		delete *it;	
	}
	Threads.clear();
	cout<<"chatting terminated, thx for use! "<<endl;
}

void chat::beginChat(){
	Threads.push_back(new thread(bind(&chat::recvMsg,this)));
	Threads.push_back(new thread(bind(&chat::sendMsg,this)));
	cout<<"threads initialization success"<<endl;
	cout<<"now lets chat!"<<endl;
	cout<<"--------------------------------------------------"<<endl;
}

void chat::recvMsg(){
	while(!ifquit){
		char recvline[MAXLINE];
		int len;		
		if((len=recv(sockfd,recvline,MAXLINE,0))>0){   //此时可能是接收到消息被唤醒，也可能是要结束程序了
			if(ifquit) continue; 
			unique_lock<mutex> lk(m_mutex);
			recvline[len] = '\0';                  //传过来的字符是不带结束符的，所以这一步很重要！
			cout<<"message from the other:"<<endl;			
			cout<<recvline<<endl;		
			cout<<"--------------------------------------------------"<<endl;
			if(strcmp(recvline,"quit") == 0){
				ifquit = true;	
			}
		}
		//this_thread::sleep_for(chrono::seconds(1));
	}
}


void chat::sendMsg(){
	while(!ifquit){
		char sendline[MAXLINE];
		fgets(sendline,MAXLINE,stdin);
		unique_lock<mutex> lk(m_mutex);
		
		sendline[strlen(sendline)-1] = '\0'; //最后一个回车键改为结束符
		
		cout<<"sending message:  "<<sendline<<" to the other"<<endl;
		if(send(sockfd,sendline,strlen(sendline),0) == -1){         
			cout<<"send msg error"<<endl;		
		}
	
		if(strcmp(sendline,"quit") == 0){
			cerr<<"quitting..."<<endl;
			ifquit = true;
			continue;		
		}
		
		cout<<"--------------------------------------------------"<<endl;
	}
}


