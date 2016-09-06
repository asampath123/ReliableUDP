
#include<stdio.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<string.h>
#include "Server.h"
#include <bitset>
#include <sstream>

typedef unsigned char byte;

using namespace std;

int main(int argc, char *argv[]){
	Server server;

	if (argc!=2){
				printf("Please provide correct input :: ./ProgramName port_number");
				exit(1);
			}
	int portNUmber=atoi(argv[1]);
	cout << "!!!Port number is" << argv[1] << endl; // prints !!!Hello World!!!

		Header headerObject;
		Header header;
		int socketFiledescriptor;
		socketFiledescriptor=server.initiateServer(portNUmber);
		printf("\n sfd is %d",socketFiledescriptor);
		byte *requestBuffer= (byte*) calloc(MAX_SEGMENT_SIZE,sizeof(byte*));
				while (1)
					{
					server.recieveFrom(requestBuffer);
					headerObject=header.ProcessRequest(requestBuffer,headerObject);
					if(headerObject.content)
					{
					//server.openFile(headerObject.content);
					server.operateFileAndSegment(headerObject.content);
					}
					cout<<"waiting for next request"<<endl;
					}
		printf("\n end main \n");
		close(socketFiledescriptor);
		printf("exiting main \n");
		return 0;

}
