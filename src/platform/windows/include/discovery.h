#ifndef DISCOVERY
#define DISCOVERY

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 42422
#define BACKLOG 16

typedef struct {
    char id[37];
    char username[64];
    char ip[16];
    int  port_tcp;
    char message[128];
    time_t last_time;
} devices;


int presence(char *ip,int port_tcp,char *message,char *username,int id); 
char * id_generertor();
#endif