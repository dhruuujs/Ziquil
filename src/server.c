#include<stdio.h>
#include<stdlib.h>
#include "../include/serverutils.h"


int main(){
    int res;
    while(1){

    printf("\n\t---Server Menu---\n");
    printf("1)Enter 1 to start server.\n2)Enter 2 to check version.\n");
    printf("3)Enter 3 to check server configuration.\n4)Enter 4 exit server\n:->");

    if (scanf("%d", &res) != 1) {
        fprintf(stderr, "Invalid option. Please enter a number.\n");
        return EXIT_FAILURE;
    }
   switch(res){
    case 1: ziInitServer(); break;
    case 2: ziGetVersion(); break;
    case 4: return EXIT_SUCCESS;
    default: printf("Failed to start server"); break;
};


    }
   return EXIT_SUCCESS;
}
