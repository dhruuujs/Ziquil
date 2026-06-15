#include<string.h>
#include<stdio.h>
#include "serverutils.h"
#include<winsock2.h>


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



void serveFile(SOCKET sockfd,const char *requestedFile){
    char resHeader[512];
    const char *fullFileName = requestedFile+1;
    
    printf("User requested file:%s\n",fullFileName);

    FILE *fptr;
    fptr = fopen(fullFileName,"rb");

    if(fptr==NULL){
    fptr = fopen("FileNotFound.html","rb");

    if(fptr==NULL){
        printf("What ya tryna read,bruhh!\n");
    }
    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char html[fileSize];
    const char *body="<h1>Error 404</h1><br/><h2>File not found!</h2>";

    int byteRead = fread(html,1,sizeof(html),fptr);
      sprintf(resHeader,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n",
            "\r\n",
            strlen(body));

        int sendFileHeader = send(sockfd,resHeader,strlen(resHeader),0);
        int sendFileNotFound = send(sockfd,body,strlen(body),0);
        
        if(sendFileNotFound>0){
            printf("Server says:Sent 404 Page.\n");
        }
            printf("Server says:404 Error.File not found!\n");
    }


    fseek(fptr,0,SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    char html[fileSize+1];

    int totalByteRead = fread(html,1,fileSize,fptr);

    sprintf(resHeader,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type:%s\r\n"
    "Content-Length:%ld\r\n"
    "\r\n",
    checkMime(fullFileName),
    fileSize);

    int sendHeaderBytes = send(sockfd,resHeader,strlen(resHeader),0);
    int sendFileBytes = send(sockfd,html,totalByteRead,0);
    if(sendFileBytes>0){
        printf("Server says:Server sent the files\n");
    }else{
        printf("[%d]",sendFileBytes);
        printf("Server says:Failed to send files\n");
    }

fclose(fptr);
}


