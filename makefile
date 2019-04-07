all:myChatRoom myChatRoomServer

myChatRoom:myChatRoom.o chat.o
	g++ -g -o myChatRoom chat.o myChatRoom.o -pthread
myChatRoomServer:myChatRoomServer.o chatServer.o
	g++ -g -o myChatRoomServer chatServer.o myChatRoomServer.o -pthread
chat.o:chat.cpp
	g++ -g -c -std=c++11 chat.cpp
chatServer.o:chatServer.cpp
	g++ -g -c -std=c++11 chatServer.cpp
myChatRoom.o:myChatRoom.cpp
	g++ -g -c -std=c++11 myChatRoom.cpp
myChatRoomServer.o:myChatRoomServer.cpp
	g++ -g -c -std=c++11 myChatRoomServer.cpp

clean:
	rm *.o myChatRoom myChatRoomServer

