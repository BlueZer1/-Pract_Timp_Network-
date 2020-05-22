#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
using namespace std;

void errHandler (const char * why, const int exitCode = 1)
{
	cerr<< why << endl;
	exit(exitCode);
	}
int main(int argc, char **argv)
{
	sockaddr_in * selfAddr = new (sockaddr_in);
	selfAddr -> sin_family = AF_INET;
	selfAddr -> sin_port = 0;
	selfAddr -> sin_addr.s_addr = 0;

	sockaddr_in * remoteAddr = new (sockaddr_in);
	remoteAddr -> sin_family = AF_INET;
	remoteAddr -> sin_port = htons(7);
	remoteAddr -> sin_addr.s_addr = inet_addr ("82.179.90.12");

	char *buf = new char [256];
	strcpy(buf, "HEY!!!! HOW ARE U?\n");
	int msglen = strlen(buf);
	

	int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mySocket == -1) {
		errHandler( "Error open socket!!!", 11);
	}

	int rc = bind (mySocket, (const sockaddr*) selfAddr, sizeof(sockaddr_in));
	if (rc == -1) {
		close(mySocket);
		errHandler("Error bind socket with local address", 12);
	}

	rc = connect (mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
	if (rc == -1) {
		close(mySocket);
		errHandler( "Error connect socket to local address", 13);
	}
	
	rc = send (mySocket, buf, msglen, 0);
	if (rc == -1) {
		close(mySocket);
		errHandler( "Error send message", 14 );
	}
	cout << "We send : " << buf << endl;
	
	rc = recv (mySocket, buf, 256, 0);
	if (rc == -1) {
		close(mySocket);
		errHandler("Error receive answer",15);
	}
	buf[rc]='\0';
	cout << "We recieve : " << buf << endl;
	close (mySocket);
	delete selfAddr;
	delete remoteAddr;
	delete[]buf;
	return 0;
}