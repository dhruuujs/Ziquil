#include<winsock2.h>
#include<stdio.h>
#include<windows.h>
#include<ws2tcpip.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include "serverutils.c"


#define PORT 8080//server port
#define BUFFER_SIZE 2048



void display();
void handleClient(SOCKET clientSoc,char *inputBuff);

int main(){
    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET socketfd = INVALID_SOCKET;
    
    int wsaErr;
    int clientAddr_size;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char data[BUFFER_SIZE];




    wVersionRequested = MAKEWORD(2,2);
    wsaErr = WSAStartup(wVersionRequested,&wsaData);

    if(wsaErr!=0){
        printf("WSAStartup failed with error:%d\n",wsaErr);
        WSACleanup();
        return 1;
    }

   socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd==INVALID_SOCKET){
        printf("Failed to create socket.\nProgram exited with error code:%d",WSAGetLastError());
    }

    server.sin_family= AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=INADDR_ANY;


    int bindInfo =  bind(socketfd,(struct sockaddr*)&server,sizeof(server));
    printf("Server binded to socket\n");
    if(bindInfo!=0){    
        printf("Server failed to bind to socket.\n Exited with error code:%d",bindInfo);
        return 1;
    }


    u_long mode= 1;
    if(ioctlsocket(socketfd,FIONBIO,&mode)==SOCKET_ERROR){
        printf("Failed:%d\n",WSAGetLastError());
    }
    

    int listenInfo = listen(socketfd,5);//returns 0 on success
    if(listenInfo == SOCKET_ERROR){
        printf("Can't listen for incoming clients");
    }else{
        printf("Server listening at port:%d\n",PORT);
    }

    while(1){
    clientAddr_size = sizeof(client);

    SOCKET acceptClient = accept(socketfd,(struct sockaddr*)&client,&clientAddr_size);
    if(acceptClient==INVALID_SOCKET){
        if(WSAGetLastError()==WSAEWOULDBLOCK){
            Sleep(100);
            continue;
        }
    printf("Accept failed:%d\n",WSAGetLastError());
    break;
    }
printf("Handling client now!\n");

int recByte =recv(acceptClient,data,BUFFER_SIZE-1,0);

if(recByte>0){
    //printf("Received data:%s\n",&data);
    data[recByte] = '\0';
    handleClient(acceptClient,data);
}else if(recByte==0){
    printf("Connection Closed\n");
}else{
    printf("Received failed with error:%d\n",WSAGetLastError());
}

wsaErr = closesocket(acceptClient);
if(wsaErr==SOCKET_ERROR){
    printf("Close failed with error:%d\n",WSAGetLastError());
}

}//while loop

    closesocket(socketfd);
    WSACleanup();
    return 0;     
}//main

void display(){
    printf("When i was a little child, i always wanted to manage the internet. So i grew up to be a web-server ^_____^\r\n");
}


void handleClient(SOCKET clientSoc,char *inputBuff){
char method[16];
char path[256];
char resHeader[512];

    sscanf(inputBuff,"%15s %200s",method,path);
    //printf("Method:%s\n",method);
    printf("Path:%s\n",path+1);
    serveFile(clientSoc,(const char*)path);     

}
