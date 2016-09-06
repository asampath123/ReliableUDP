/*
 * Server.cpp
 *
 *  Created on: Sep 30, 2015
 *      Author: abhinandan
 */

#include "Server.h"
#include<string>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<fstream>

#include<sstream>
struct timeval tv;
struct timezone tz;

Header Header::formHeader(int seq,byte fin,int length,byte *data){
	Header headerObj;
	headerObj.sequenceNumber=seq;
	headerObj.ack=seq+1;
	headerObj.ackFLag='Y';
	headerObj.finFLag=fin;
	headerObj.lenghtOfFile=length;
	cout<<"length of file"<<headerObj.lenghtOfFile<<endl;
	headerObj.content=data;
	return headerObj;

}


//create respone base on the request
byte * Server::createResponse(Header header){
	//printf("inside createResponse");
	int shiftCount=0;
		Header headerobj;
		byte* buffer= (byte*) calloc(MAX_SEGMENT_SIZE,sizeof(byte*));
		memset(buffer,0,MAX_SEGMENT_SIZE);

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

	/*
	byte *contentOfFile=(byte*) calloc(MAX_SEGMENT_SIZE,sizeof(byte*));
	memcpy(contentOfFile,header.content,MAX_SEGMENT_SIZE);
	return contentOfFile;*/
}

	//start the server
int Server::initiateServer(int port){
		//int socketFiledescriptor;
		struct sockaddr_in server;
		socketFiledescriptor=socket(PF_INET,SOCK_DGRAM,0);
			if(socketFiledescriptor < 1)
				{
					printf("could not create socket");
				}
			bzero((char *) &server, sizeof(server));
			server.sin_addr.s_addr = inet_addr("127.0.0.1");
			server.sin_family = AF_INET;
			server.sin_port = htons(port);
				 if (bind(socketFiledescriptor, (struct sockaddr *) &server, sizeof(server)) < 0)
				    {
					 printf("\n ERROR during binding,please change the port number");
					 exit(1);
				    }
				    printf("\n starting the UDP server");
				    return socketFiledescriptor;
	}

//parse the httprequest for details
Header Header::ProcessRequest(byte * requestBuffer,Header header){
			//printf("\n inside HttpRequest \n");

			Header headerobj;
			Server server;
			headerobj.sequenceNumber=0;

			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[3];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[2];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[1];
			headerobj.sequenceNumber = (headerobj.sequenceNumber << 8)|requestBuffer[0];
			//cout <<"headerobj.sequenceNumber" <<headerobj.sequenceNumber<<endl;

			headerobj.ack = (headerobj.ack << 8)|requestBuffer[7];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[6];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[5];
			headerobj.ack = (headerobj.ack << 8)|requestBuffer[4];
			//cout <<"headerobj.ack" <<headerobj.ack<<endl;

			headerobj.ackFLag =(headerobj.ackFLag << 8)|requestBuffer[8];
			//cout <<"headerobj.ackFLag" <<headerobj.ackFLag<<endl;
			//cout << "1.headerobj.ackFLag is" << bitset<8>(requestBuffer[8])<< endl;
			//cout << "2.headerobj.ackFLag is" << bitset<8>(headerobj.ackFLag) << endl;

			headerobj.finFLag= (headerobj.finFLag << 8)|requestBuffer[9];
			//cout <<"headerobj.finFLag" <<headerobj.finFLag<<endl;

			headerobj.lenghtOfFile= (headerobj.lenghtOfFile << 8)|requestBuffer[10];
			headerobj.lenghtOfFile= (headerobj.lenghtOfFile << 8)|requestBuffer[11];
			//cout <<"headerobj.lenghtOfFile is" <<headerobj.lenghtOfFile<<endl;


			headerobj.content= &requestBuffer[12];
			//cout <<"filename is" <<requestBuffer[12]<<endl;
			//cout <<"filename is" <<headerobj.content<<endl;

			//headerobj.content=server.getFileContent(headerobj.content);

			return headerobj;
			//call file and copy data from file;
			//prepare
}

void Server::prepareSegmentsAndSend(int seqNumber, byte finFLag,int first, int last){
				Server server;
				int sendToFlag;
				byte *packet=(byte *)calloc(MAX_SEGMENT_SIZE,sizeof(byte *));
				int filelength=last-first;
				//cout<<"filelength is"<<filelength;
				char *data=(char *)calloc(filelength,sizeof(char *));
				byte fin=finFLag;
				//cout<<"file status is"<<fpointer.is_open()<<"2"<<this->fpointer.is_open()<<endl;
				if(fpointer.is_open()) {
					//cout<<"is open"<<endl;
					fpointer.seekg(first,ios::beg);
					fpointer.read(data,filelength);
					}
				else
				{
					//cout<<"error opening the file,inside readAndSend"<<endl;
					return;
					//exit(1);
				}
				byte* content=(byte*)data;

				header=header.formHeader(seqNumber,fin,filelength,content);
				//cout<<"header.sequenceNumber is"<<header.sequenceNumber<<endl;
				packet=server.createResponse(header);
				sendToFlag=sendToCLient(packet);
				if(sendToFlag){
					printf("sent to client \n");
				}
}



void Server::recieveFrom(byte *requestBuffer){
	bzero(requestBuffer,MAX_SEGMENT_SIZE);
	struct sockaddr_in client;
	socklen_t clientLength;
	int n;
	//cout << "receive from client addr :: " << client.sin_addr.s_addr;
	//cout << "client port :: " << client.sin_port << endl;
	clientLength=sizeof(client);
	n=recvfrom(socketFiledescriptor,requestBuffer,MAX_SEGMENT_SIZE,0,(struct sockaddr *)&client,&clientLength);
	printf("bytes length received %d \n",n);
						//keep reading open for UDP.
	printf("received from client \n");
						//get request details
	clientAddress=client;
}

bool Server::sendToCLient(byte* packet){
	int n;
	//cout << "sending to client addr :: " << clientAddress.sin_addr.s_addr;
	//cout << "client port :: " << clientAddress.sin_port << endl;
	//cout<<"socketFiledescriptor is"<<socketFiledescriptor<<endl;
	n=sendto(socketFiledescriptor,packet,MAX_SEGMENT_SIZE,0,(struct sockaddr *)&clientAddress,sizeof(clientAddress));
						gettimeofday(&tv, &tz);
						//printf("Last packet sent at %d\n",tv.tv_usec);

						//printf("waiting for next request......\n");
						if(n<-1)
						{
							printf("error sending to client");
							return false;
						}
	return true;
}




void Server::operateFileAndSegment(byte* filename){
	//Header header;
	Server server;
	char *fname=(char*)filename;
	int fileLength;
	int count=0;
	int seqNumber=2345;
	byte finFLag='N';
	int first=0;
	int last=0;


	fpointer.open(fname,std::ios::in);
	if (!fpointer.is_open()) {
		cout << "file open failure for "<<fname<<endl;
					}
	fpointer.seekg(0,ios::end);
	fileLength = fpointer.tellg();
	cout<<"filelength is"<<fileLength<<endl;
	fpointer.seekg(0,ios::beg);

			if(fileLength <= MAX_SEGMENT_DATA_SIZE) {
					handleSingleSegment(seqNumber,fileLength);
					}
			else{
	int segmentsCount=fileLength/MAX_SEGMENT_DATA_SIZE+1; //integer fraction, +1 for whole number
	cout<<"seg count is"<<segmentsCount<<endl;
	while(1){
		if(count!=segmentsCount){
			if(count+1!=segmentsCount){
				cout<<"seqNumber is"<<seqNumber<<endl;
				last=last+MAX_SEGMENT_DATA_SIZE;
				prepareSegmentsAndSend(seqNumber,finFLag,first,last);
				if(!chekforAck(seqNumber)){
						cout<<"ack not recieved"<<endl;
						exit(0);
						}

				//cout<<"else last"<<endl;
				}
			else{
				finFLag='Y';
				cout<<"seq number is"<<seqNumber<<endl;
				cout<<"last packet"<<endl;
				last=fileLength;
				prepareSegmentsAndSend(seqNumber,finFLag,first,last);
				if(!chekforAck(seqNumber)){
						cout<<"ack not recieved"<<endl;
						exit(0);
					}
			}
			seqNumber=seqNumber+MAX_SEGMENT_DATA_SIZE;
			count=count+1;
			cout<<"total segments -------"<<segmentsCount<<endl;
			cout<<"current count---------"<<count<<endl;
			first=last;

		}
		else{
			fpointer.close();
			//cout<<"break"<<endl;
			break;
		}
	}
			}
}

void Server::handleSingleSegment(int seqNumber,int fileLength){
		cout<<"handleSingleSegment"<<endl;
		byte finFLag='Y';
		prepareSegmentsAndSend(seqNumber, finFLag,0,fileLength);
		fpointer.close();
		if(chekforAck(seqNumber)){
		return;}
		else{
			cout<<"ack not recieved"<<endl;
			exit(0);
		}

}

bool Server::chekforAck(int seqNumber){
		byte* requestBufferforAck=(byte*)calloc(MAX_SEGMENT_SIZE,sizeof(byte));
		bzero(requestBufferforAck,MAX_SEGMENT_SIZE);
		struct sockaddr_in client;
		socklen_t clientLength;
		int n;
		//cout << "receive from client addr :: " << client.sin_addr.s_addr;
		//cout << "client port :: " << client.sin_port << endl;
		clientLength=sizeof(client);

		cout<<"waiting for ack"<<endl;
		cout<<"expected ack is"<<seqNumber+MAX_SEGMENT_DATA_SIZE<<endl;

		/*while(1){
		n=recvfrom(socketFiledescriptor,requestBufferforAck,MAX_SEGMENT_SIZE,0,(struct sockaddr *)&client,&clientLength);
		if(n){
			break;
		}
		}*/
		n=recvfrom(socketFiledescriptor,requestBufferforAck,MAX_SEGMENT_SIZE,0,(struct sockaddr *)&client,&clientLength);
		header=header.ProcessRequest(requestBufferforAck,header);
		if(header.ack==seqNumber+MAX_SEGMENT_DATA_SIZE)
		{
			cout<<"ack received is"<<header.ack<<endl;
			return true;
		}
		return false;


}

Server::Server(){

}


Server::~Server(){
	// TODO Auto-generated destructor stub
}

