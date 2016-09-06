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
#include "Client.h"
using namespace std;
typedef basic_stringstream<char>stringstream;
#include <bitset>

typedef unsigned char byte;

int main(int argc, char *argv[]){
	struct timeval tv;
	struct timezone tz;
	Header headerObject;
	Header header;
	printf("start client");
		ServerDetails serverDetailsObj;
		Client client;
		//byte *completeRequest;
		int sendtoStatus;
		int i;
		int recvFrom;
		int ack;
		//byte *sendingbuffer=(byte*) calloc(1472,sizeof(byte*));
		byte *receiveBuffer=(byte*) calloc(1472,sizeof(char*));
		if (argc!=4){
			printf("Please provide correct input :: hostname,port,filepath url");
			exit(1);
		}
		for (i=1; i< argc; i++) {
		     printf("\n arg %d =%s", i, argv[i]);
		 }
		printf("\n arguments received \n");
		printf("argv 1 is %s \n",argv[1]);
		printf("port number is %s \n",argv[2]);
		printf("filename is %s \n",argv[3]);
		byte* filename=(byte*) argv[3];
		cout<<"filename is" <<filename<<endl;

		byte *completeRequest=(byte*) calloc(1472,sizeof(byte *));	//temp init
		serverDetailsObj=client.setUpConnectionWithServer(argv[1],argv[2]);
		completeRequest=client.sendRequestToServer(filename);


		//send to the server
		sendtoStatus = sendto(serverDetailsObj.socketFileDesc, completeRequest,1472,0,((sockaddr*)&(serverDetailsObj.serverAddress)), sizeof(struct sockaddr_in));
		if (sendtoStatus < 0) {
			printf("ERROR writing to socket\n");
		}
		while(1){
			bzero(receiveBuffer,1472);
					//recieve from the server
					recvFrom= recvfrom(serverDetailsObj.socketFileDesc, receiveBuffer, 1472, 0 ,NULL, NULL);
					headerObject=header.ProcessReceived(receiveBuffer,headerObject);
					if(headerObject.finFLag=='Y'){

						ack=headerObject.sequenceNumber+1460;
						completeRequest=client.sendACKToServer(ack);
						cout<<"sending ack "<<ack<<" for seqNUmber"<<headerObject.sequenceNumber<<endl;
						sendtoStatus = sendto(serverDetailsObj.socketFileDesc, completeRequest,1472,0,((sockaddr*)&(serverDetailsObj.serverAddress)), sizeof(struct sockaddr_in));
						if (sendtoStatus < 0) {
									printf("ERROR writing to socket - during ack to server\n");
								}
						cout<<"fin flag true - terminating connection \n";
						gettimeofday(&tv, &tz);
						printf("Time of last packet received at %d\n",tv.tv_usec);
						printf("number of bytes received is %d \n",recvFrom);
						break;
					}
					ack=headerObject.sequenceNumber+1460;
					completeRequest=client.sendACKToServer(ack);
					cout<<"sending ack "<<ack<<" for seqNUmber"<<headerObject.sequenceNumber<<endl;
					sendtoStatus = sendto(serverDetailsObj.socketFileDesc, completeRequest,1472,0,((sockaddr*)&(serverDetailsObj.serverAddress)), sizeof(struct sockaddr_in));
					gettimeofday(&tv, &tz);
					printf("Time of last packet received at %d\n",tv.tv_usec);
					printf("number of bytes received is %d \n",recvFrom);
					if (recvFrom < 0) {
					printf("ERROR reading from socket\n");
					}
						printf("*Response recieved*......\n");
		}


	close(serverDetailsObj.socketFileDesc);
	return 0;
}
