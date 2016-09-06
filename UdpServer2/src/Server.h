/*
 * Server.h
 *
 *  Created on: Sep 30, 2015
 *      Author: abhinandan
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <bitset>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<netdb.h>
#include<netinet/in.h>
#include<time.h>
#include<sys/time.h>
#include<fstream>
using namespace std;
typedef unsigned char byte;
const int MAX_SEGMENT_SIZE=1472;
const int MAX_SEGMENT_DATA_SIZE=1460;


class Header{
public:
	int sequenceNumber;
	int ack;
	byte finFLag;
	byte ackFLag;
	short lenghtOfFile;
	byte* content;

	Header ProcessRequest(byte * requestBuffer,Header header);
	Header formHeader(int sequenceNumber, byte finFLag,int filelength,byte* content);

};

//struct sockaddr_in clientAddress;
class Server {

	Header header;
	fstream fpointer;
	struct sockaddr_in clientAddress;
	int socketFiledescriptor;
public:


	Server();
	byte *createResponse(Header header);
	int initiateServer(int port);
	void recieveFrom(byte *requestBuffer);
	struct httpRequest HttpRequest(byte * requestBuffer,struct httpRequest httpRequestObject);
	void operateFileAndSegment(byte* filename);
	void prepareSegmentsAndSend(int seqNumber, byte finFLag,int first, int last);
	void handleSingleSegment(int seqNumber,int fileLength);
	bool sendToCLient(byte* packet);
	bool chekforAck(int seqNumber);
	virtual ~Server();
};

#endif /* SERVER_H_ */
