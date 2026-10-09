#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H
#include<winsock2.h>

int ziInitServer();
void ziGetVersion();



const char* checkMime(const char *file);
int serveFile(SOCKET sockfd,const char *requestedFile);

typedef struct{
    int resCode; 
    char resChars[500];
}ResType;


typedef struct{
    char data[1024];
    int maxAge;//use Expires:Thu, 31 Oct 2021 07:28:00 GMT or Max-Age:58893487444 attr
}Cookies;



typedef struct{
    char *host;
    int port;
    char *httpServePath;
    int health;
}ServerConfigs;



typedef enum {
    CONTINURE=100,
    SWITCH_PROTOCOL=101,
    EARLY_HINTS=103,

    OK=200,
    CREATED=201,
    ACCEPTED=202,

    MULTIPLE_CHOICE=300,
    MOVED_PERMANENTLY=301,
    FOUND=302,

    BAD_REQUEST=400,
    UNAUTHORIZED=401,
    FORBIDDEN=403,
    NOT_FOUND=404,
    METHOD_NOT_ALLOWED=405,
    URI_TOO_LONG=414,
    IM_A_TEA_POT=418,
    ERROR_TOO_MANY_REQUEST=429,
    REQUEST_HEADER_TOO_LONG=431,

    INTERNAL_SERVER_ERROR=500,
    NOT_IMPLEMENTED=501,
    BAD_GATEWAY=502,
    SERVICE_UNAVAILABLE=503,
    GATEWAY_TIMEOUT=504,
    HTTP_VERSION_NOT_SUPPORTED=505,
    INSUFFIENT_STORAGE=507
}statusCode;


/*
Cookie architecture


*/



extern ResType resStruct;
extern Cookies cookies;
extern statusCode httpCode;
extern ServerConfigs serverConfigs;
#endif