#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <stdio.h>
using namespace std;

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

void error(const char *msg)
{
   perror(msg);
   exit(0);
}

int main(int argc, char *argv[])
{
/************** gpio ********************/
   wiringPiSetup();
pinMode(0,INPUT);// pin 11
pinMode(1,INPUT);	//pin12
pinMode(2,INPUT);//pin14
pinMode(3,INPUT);//pin 15

int flag = 0; 

cout << "before the start" << endl;
cout << digitalRead(0) << endl;
cout << digitalRead(1) << endl;
if(digitalRead(0))
cout << "on the sensor1" << endl;
else
cout << "off the sensor1" << endl;

if(digitalRead(1))
cout << "on the sensor2" << endl;
else
cout << "off the sensor2" << endl;

while(!(!digitalRead(0) && !digitalRead(1)));

cout << "after the start" << endl;

/************************ client **************************************/

int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[2];
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
   portno = atoi(argv[2]);
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
       error("ERROR opening socket");

   server = gethostbyname(argv[1]);

   if (server == NULL) {
       fprintf(stderr,"ERROR, no such host\n");
       exit(0);
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;

   bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);

   serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

   serv_addr.sin_port = htons(portno);
   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
       error("ERROR connecting");

string prev = "9";
       string str;
while (1)
{

//if( !(!digitalRead(0) && !digitalRead(1))) // if you get off any of the Sensors
 	
       str = "0";	 //prev buffer


bzero(buffer,2);



if(digitalRead(0))
str ="1";
else
str = "0";
              

if(digitalRead(1))
{
switch(str[0])
{
case '1':str = "3";
break;
case '0':str = "2";
break;
}

}


if(digitalRead(2))
{
switch(str[0])
{
case '0':str = "4";
break;
case '1':str = "5";
break;
case '2':str = "6";
break;
case '3':str = "7";
break;
}
}


//string str = "99";

if(str.compare(prev) != 0)
{
prev = str;
size_t len = str.copy(buffer,1,0);
buffer[1] = '\0';

//pause();
               n = write(sockfd,buffer,strlen(buffer));
if (n < 0) 
        error("ERROR writing to socket");
               //while(!(!digitalRead(0) && !digitalRead(1)));//

//pause();
//n = write(sockfd,buffer,strlen(buffer));
//if (n < 0) 
       	 //error("ERROR writing to socket");
}
      
     usleep(2000);
}

   close(sockfd);
   return 0;
}