#ifndef NETWORK
#define NETWORK


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <mswsock.h>


#define SERVER_PORT 42422
#define BACKLOG 16

//Prototype des fonctions
SOCKET init_tcp(int port);
SOCKET accept_client(SOCKET server_fd,struct sockaddr*client_addr);
SOCKET connect_to(const char * ip,int port);
int network_send_struct(SOCKET fd,const void * data,size_t size);
int network_recv_struct(SOCKET fd,const void * data,size_t size);


#endif