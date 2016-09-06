/*
 * Client.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: abhinandan
 */

#include "Client.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <err.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
using namespace std;



Header Header::ProcessReceived(byte * requestBuffer,Header header){
			printf("\n inside HttpRequest \n");

			Header headerobj;
			//Client server;
			//headerobj=decodeHeader(requestBuffer);
			headerobj.sequenceNumber=0;

			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[3];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[2];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[1];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[0];
			cout <<"headerobj.sequenceNumber" <<headerobj.sequenceNumber<<endl;

			headerobj.ack = (headerobj.ack << 8)|requestBuffer[7];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[6];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[5];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[4];
			cout <<"headerobj.ack" <<headerobj.ack<<endl;

			headerobj.ackFLag =(headerobj.ackFLag << 8)|requestBuffer[8];
			cout <<"headerobj.ackFLag" <<headerobj.ackFLag<<endl;
			//cout << "1.headerobj.ackFLag is" << bitset<8>(requestBuffer[8])<< endl;
			//cout << "2.headerobj.ackFLag is" << bitset<8>(headerobj.ackFLag) << endl;

			headerobj.finFLag= (headerobj.finFLag << 8)|requestBuffer[9];
			cout <<"headerobj.finFLag" <<headerobj.finFLag<<endl;

			headerobj.lenghtOfFile= (headerobj.lenghtOfFile << 8)|requestBuffer[11];
			headerobj.lenghtOfFile= (headerobj.lenghtOfFile << 8)|requestBuffer[10];
			cout <<"headerobj.lenghtOfFile is" <<headerobj.lenghtOfFile<<endl;


			headerobj.content= &requestBuffer[12];
			//cout <<"content is" <<requestBuffer[12]<<endl;
			cout <<"content is"<<endl;
			cout<<headerobj.content<<endl;
			//headerobj.content=server.getFileContent(headerobj.content);

			return headerobj;
			//call file and copy data from file;
			//prepare
}



Header Header::prepare(int sequenceNumber,int ack,char ackFLag,char finFLag,short lenghtOfFile,byte* content){
	Header headerObj;
	headerObj.sequenceNumber=sequenceNumber;
	headerObj.ack=ack;
	headerObj.ackFLag=ackFLag;
	cout<<"ackFlag is" <<ackFLag<<endl;
	headerObj.finFLag=finFLag;
	headerObj.lenghtOfFile=lenghtOfFile;

	//headerObj.lenghtOfFile=sizeof(content);
	headerObj.content=content;
	return headerObj;
}


byte* Header::headerDesign(Header header){
	int shiftCount=0;
	Header headerobj;
	byte* buffer= (byte*) calloc(1472,sizeof(byte*));
	memset(buffer,0,1472);

	headerobj.sequenceNumber=header.sequenceNumber;
	memcpy(buffer+shiftCount,&headerobj.sequenceNumber,sizeof(headerobj.sequenceNumber));
	shiftCount=shiftCount+sizeof(headerobj.sequenceNumber);
	//cout << "2.headerobj.syn is" << bitset<8>(buffer[0]) << endl;

	headerobj.ack=header.ack;
	memcpy(buffer+shiftCount,&headerobj.ack,sizeof(headerobj.ack));
	shiftCount=shiftCount+sizeof(headerobj.ack);

	headerobj.ackFLag=header.ackFLag;
	//cout <<"header.ackFLag" <<header.ackFLag<<endl;
	//cout <<"headerobj.ackFLag" <<headerobj.ackFLag<<endl;
	memcpy(buffer+shiftCount,&headerobj.ackFLag,1);
	shiftCount=shiftCount+sizeof(headerobj.ackFLag);
	//cout << "1.headerobj.ackFLag is" << bitset<8>(headerobj.ackFLag)<< endl;
	//cout << "2.headerobj.ackFLag is" << bitset<8>(buffer[8]) << endl;

	headerobj.finFLag=header.finFLag;
	memcpy(buffer+shiftCount,&headerobj.finFLag,1);
	shiftCount=shiftCount+sizeof(headerobj.finFLag);

	headerobj.lenghtOfFile=header.lenghtOfFile;
	memcpy(buffer+shiftCount,&headerobj.lenghtOfFile,sizeof(headerobj.lenghtOfFile));
	shiftCount=shiftCount+sizeof(headerobj.lenghtOfFile);

	headerobj.content=header.content;
	memcpy(buffer+shiftCount,headerobj.content,headerobj.lenghtOfFile);
	//printf("*** Final Request being sent:%s\n",buffer);
	//cout << "headerobj.content is" << bitset<32>(buffer[8]) << endl;
	//cout << "headerobj.content is" << headerobj.content << endl;

	//cout <<"buffer is"<<buffer<<endl;
	//puts(buffer);
	return buffer;

}

byte* Client::sendACKToServer(int ack){
			Header header;
			Header Headerobj;
			byte *tempbuffer=(byte*) calloc(1472,sizeof(byte*));
			byte *sendingbuffer=(byte*) calloc(1472,sizeof(byte*));	//temp init
			bzero(sendingbuffer,0);
			int seqNum=0;
			int ackNum=ack;
			byte flagAck='Y';
			byte flagFin='N';
			int fileLength=0;
			byte *file;

			header=header.prepare(seqNum,ackNum,flagAck,flagFin,fileLength,file);

			//send this sending buffer to receiver from intiateServer method
			tempbuffer=header.headerDesign(header);
			memcpy(sendingbuffer,tempbuffer,1472);
			//cout << "2.headerobj.syn is" << bitset<8>(sendingbuffer[0]) << endl;
			//cout << "2.headerobj.ackFLag is" << bitset<8>(sendingbuffer[8]) << endl;
			return sendingbuffer;
}
byte* Client::sendRequestToServer(byte *filename)
	{
		Header header;
		Header Headerobj;
		byte *tempbuffer=(byte*) calloc(1472,sizeof(byte*));
		byte *sendingbuffer=(byte*) calloc(1472,sizeof(byte*));	//temp init
		bzero(sendingbuffer,0);
		int seqNum=1234;
		int ackNum=1235;
		byte flagAck='Y';
		byte flagFin='N';
		int fileLength=20;
		cout<<"inside sendrequest filename is" <<filename<<endl;
		byte *file=filename;

		header=header.prepare(seqNum,ackNum,flagAck,flagFin,fileLength,file);

		//send this sending buffer to receiver from intiateServer method
		tempbuffer=header.headerDesign(header);
		memcpy(sendingbuffer,tempbuffer,1472);
		//cout << "2.headerobj.syn is" << bitset<8>(sendingbuffer[0]) << endl;
		//cout << "2.headerobj.ackFLag is" << bitset<8>(sendingbuffer[8]) << endl;
		return sendingbuffer;
}

ServerDetails Client::setUpConnectionWithServer(char *serverHostName,char *portNumber)
	{
		printf("inside setUpConnectionWithServer \n");
		ServerDetails serverDetailsObj;
		struct hostent *server;
		int socketFd;
		int port;
		port=atoi(portNumber);
		socketFd=socket(AF_INET,SOCK_DGRAM,0);
		printf("socket id is %d \n",socketFd);
		printf("port number is %d \n",port);
		if(socketFd<0)
		{
			printf("error while opening socket");
		}
			server = gethostbyname(serverHostName);
			bzero((char *) &serverDetailsObj.serverAddress,sizeof(serverDetailsObj.serverAddress));
			memcpy(&serverDetailsObj.serverAddress.sin_addr,server->h_addr_list[0],server->h_length);
			serverDetailsObj.serverAddress.sin_family=AF_INET;
			serverDetailsObj.serverAddress.sin_port = htons(port);
			serverDetailsObj.socketFileDesc=socketFd;
			return serverDetailsObj ;
	}


Client::Client() {
	// TODO Auto-generated constructor stub

}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

