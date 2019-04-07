#include "unp.h"
#include "chatServer.h"

using namespace std;

int main()
{
	chatServer myserver;
	myserver.connectClient();
	return 0;
}
