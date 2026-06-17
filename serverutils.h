#include<winsock2.h>

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

//#define FILETYPE

int zi_init_server();

const char* checkMime(const char *file);
int serveFile(SOCKET sockfd,const char *requestedFile);
//char* checkMethod(char method);
void zi_get_server_desc();

#endif