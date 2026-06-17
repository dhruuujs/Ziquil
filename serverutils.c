#include<string.h>
#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#include<conio.h>
#include<ws2tcpip.h>
#include<stdlib.h>

#include "serverutils.h"

#define PORT 8080
#define BUFFER_SIZE 2048



void handleClient(SOCKET clientSoc,char *inputBuff);



const char* checkMime(const char *contentType){
if(strstr(contentType,".html")){
    return "text/html";
}else if(strstr(contentType,".css")){
    return "text/css";
}else if(strstr(contentType,".js")){
    return "application/javascript";
}else if(strstr(contentType,".png")){
    return "image/png";
}else if(strstr(contentType,".jpg")){
    return "image/jpeg";
}else{
    return "text/plain";
}
}


int serveFile(SOCKET sockfd,const char *requestedFile){
    char resHeader[512];
    const char *fullFileName = requestedFile+1;
    
    printf("User requested file:%s\n",fullFileName);

    FILE *fptr;
    fptr = fopen(fullFileName,"rb");

    if(fptr==NULL){
    fptr = fopen("FileNotFound.html","rb");

    if(fptr==NULL){
        printf("What ya tryna read,bruhh!\n");
    return 1;
}
    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char html[fileSize+1];


    int byteRead = fread(html,1,fileSize,fptr);
      sprintf(resHeader,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "\r\n",
            checkMime(fullFileName),
            fileSize);

        int sendFileHeader = send(sockfd,resHeader,strlen(resHeader),0);
        int sendFileNotFound = send(sockfd,html,byteRead,0);
        
        if(sendFileHeader>0 && sendFileNotFound>0){
            printf("Server says:Sent 404 Page.\n");
        }
            printf("Server says:404 Error.File not found!\n");
    }


    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char *html = malloc(fileSize+1);

    int totalByteRead = fread(html,1,fileSize,fptr);

    sprintf(resHeader,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type:%s\r\n"
    "Content-Length:%ld\r\n"
    "\r\n",
    checkMime(fullFileName),
    fileSize);

    send(sockfd,resHeader,strlen(resHeader),0);
    int sendFileBytes = send(sockfd,html,totalByteRead,0);
    free(html);
    if(sendFileBytes>0){
        printf("Server says:Server sent the files\n");
    }else{
        printf("Server says:Failed to send files\n");
    return 1;
    }

fclose(fptr);
return 0;
}

void zi_get_server_desc(){
printf("Ziquil Server version:0.2\n");
}



int zi_init_server(){
    WORD  wVersionRequested;
    WSADATA wsaData;
    SOCKET socketfd = INVALID_SOCKET;

    int wsaErr;
    size_t clientAddr_size;
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

    SOCKET acceptClient = accept(socketfd,(struct sockaddr*)&client,(int*)&clientAddr_size);
    if(acceptClient==INVALID_SOCKET){
        if(WSAGetLastError()==WSAEWOULDBLOCK){
            Sleep(100);
            continue;
        }
        printf("Accept func failed:%d\n",WSAGetLastError());
        break;
    }
    printf("\nClient Addr details:%lu\n",client.sin_addr.s_addr);
    printf("Handling client now!\n");

    int recByte =recv(acceptClient,data,BUFFER_SIZE-1,0);

    if(recByte>0){
        //printf("Received data:%s\n",&data);
        data[recByte] = '\0';
        handleClient(acceptClient,data);
    }else if(recByte==0){
        printf("Connection Closed\n");
    }else{
        if(WSAGetLastError()==10035){
            Sleep(10);
        }
        printf("Received bytes:%d\n",recByte);
        printf("Received failed with error:%d\n",WSAGetLastError());
    }

    wsaErr = closesocket(acceptClient);
    if(wsaErr==SOCKET_ERROR){
        printf("Close failed with error:%d\n",WSAGetLastError());
    }

}
closesocket(socketfd);
WSACleanup();
return 0;
}


void handleClient(SOCKET clientSoc,char *inputBuff){
char method[16];
char path[256];

    sscanf(inputBuff,"%15s %200s",method,path);
    //printf("Method:%s\n",method);
    printf("Path:%s\n",path+1);
    serveFile(clientSoc,(const char*)path);     

}





