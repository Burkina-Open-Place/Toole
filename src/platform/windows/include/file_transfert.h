#ifndef FILE_TRANSFERT
#define FILE_TRANSFERT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <mswsock.h>

#define SERVER_PORT 42422
#define BACKLOG 16

int network_send_file(SOCKET fd,const char * path);
int network_recv_file(SOCKET fd,const char *dis_path);
void network_close(SOCKET fd);
int read_n(SOCKET fd,char *buffer,size_t n);
int write_n(SOCKET fd,char *buffer,size_t n);

#endif