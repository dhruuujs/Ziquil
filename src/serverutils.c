#include<string.h>
#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#include<conio.h>
#include<ws2tcpip.h>
#include<stdlib.h>

#include "../include/serverutils.h"

#define PORT 8080
#define BUFFER_SIZE 2048 //2048KB for total header size, recommened to expand to 1024*4(4KB) in size for request header and url

void handleClient(SOCKET clientSoc,char *inputBuff);

void ziGetVersion(){
    printf("Server version:1.5.0\n");
}

Cookies cookies = { "hello", 15};//Setting up data
statusCode responseCode = OK;


const char* checkMime(const char *contentType){

if(contentType=="" || contentType[0]=='\0') return "";

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
    char s1[512] = "public\\";
    char resHeader[512];
    const char *fullFileName = requestedFile ? requestedFile + 1 : "";

    if(fullFileName[0]=='\0'){
        fullFileName = "index.html";
    }

    printf("Requested path:%s \n",requestedFile);


    snprintf(s1,sizeof(s1),"public\\%s",fullFileName);
    printf("User requested file:%s\n",s1);


    /*If index.html is failed to open, it fallback to the 
    404 error page from here.*/
    FILE *fptr;
    fptr = fopen(s1,"rb");
    if(fptr==NULL){

        fptr = fopen("public\\FileNotFound.html","rb");

    if(fptr==NULL){
        printf("What ya tryna read,bruhh!\n");
    return 1;
    }

    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char *html = malloc(fileSize+1);

    if(html==NULL){
        printf("Failed to allocate memory!.");
        EXIT_FAILURE;
    }


    int byteRead = fread(html,1,fileSize,fptr);
      snprintf(resHeader,
        sizeof(resHeader),
            "HTTP/1.1 %d Not Found\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "\r\n",
            httpCode=NOT_FOUND,
            "html",
            fileSize);

        int sendFileHeader = send(sockfd,resHeader,strlen(resHeader),0);
        int sendFileNotFound = send(sockfd,html,byteRead,0);
        
        if(sendFileHeader>0 && sendFileNotFound>0){
            printf("Server says:Sent 404 Page.\n");
        }
            printf("Server says:404 Error.File not found!\n");
    
        free(html);
        fclose(fptr);
        return 1;
        }//If reading requested file failed


    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char *html = malloc(fileSize+1);
    int totalByteRead = fread(html,1,fileSize,fptr);

    snprintf(resHeader,
    sizeof(resHeader),
    "HTTP/1.1 %d OK\r\n"
    "Content-Type:%s; charset=\"utf-8\"\r\n"
    "Content-Length:%ld\r\n"
    "Set-Cookie: session_id=%s; Expires=%d\r\n"
    "\r\n",
    responseCode=OK,
    checkMime(fullFileName),
    fileSize,cookies.data,cookies.maxAge);



    send(sockfd,resHeader,strlen(resHeader),0);//Sending the header
    int sendFileBytes = send(sockfd,html,totalByteRead,0);//Sending the body
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


int ziInitServer(){
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

    printf("Data printing now %lu\n",INADDR_ANY);
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

    char ip_str[INET_ADDRSTRLEN];
    //inet_ntop(AF_INET,&(client.sin_addr.s_addr),ip_str,INET_ADDRSTRLEN);


    while(1){
    clientAddr_size = sizeof(client);

    SOCKET acceptClient = accept(socketfd,(struct sockaddr*)&client,(int*)&clientAddr_size);

    //Printing client ip
    //Added functin inet_ntop on 12:01PM, 6/10/26
    inet_ntop(AF_INET,&(client.sin_addr.s_addr),ip_str,INET_ADDRSTRLEN);
    printf("Client %s connected.\n",ip_str);
    

    if(acceptClient==INVALID_SOCKET){
        if(WSAGetLastError()==WSAEWOULDBLOCK){
            Sleep(100);
            continue;
        }
        printf("Accept func failed:%d\n",WSAGetLastError());
        break;
    }
    printf("Handling client now!\n");

/*
@Call Threads to handle the conn here!!
*/
    int recByte; 
    int retries=0;

    while(1){
        recByte = recv(acceptClient,data,BUFFER_SIZE-1,0);

        if(recByte>0) break;
        
        if(recByte==0){
            printf("Connection Closed\n");
        break;
        }
        
        if(WSAGetLastError()==WSAEWOULDBLOCK){    
                Sleep(100);
            retries++;
            if(retries==100){
                printf("Funtion recv() timed out for client data!\n");
                break;
            }
            continue;  
        }
        printf("Received failed with error:%d\n",WSAGetLastError());
        break;
    }


    if(recByte>0){
        data[recByte]='\0';
        handleClient(acceptClient,data);
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
    
    if(serveFile(clientSoc,(const char*)path)!=0){
        perror("Failed to serve files.\n");
    }
}


