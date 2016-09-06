/*
 * Client.h
 *
 *  Created on: Oct 1, 2015
 *      Author: abhinandan
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
typedef unsigned char byte;
#include <arpa/inet.h>
#include <bitset>
#ifndef CLIENT_H_
#define CLIENT_H_

class ServerDetails {
public:
	struct sockaddr_in serverAddress;
	int socketFileDesc;
};

class Header{
public:
	int sequenceNumber;
	int ack;
	byte ackFLag;
	byte finFLag;
	short lenghtOfFile;
	byte* content;

	Header prepare(int sequenceNumber,int ack,char ackFLag,char finFLag,short lenghtOfFile,byte* content);
	byte* headerDesign(Header header);
	Header ProcessReceived(byte * requestBuffer,Header header);

};

class Client {
public:
	Client();
	byte* sendRequestToServer(byte* filename);
	ServerDetails setUpConnectionWithServer(char *serverHostName,char *portNumber);
	byte* sendACKToServer(int ack);

	virtual ~Client();
};

#endif /* CLIENT_H_ */
