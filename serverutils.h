#include<winsock2.h>

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

//#define FILETYPE

const char* checkMime(const char *file);
int serveFile(SOCKET sockfd,const char *requestedFile);

#endif