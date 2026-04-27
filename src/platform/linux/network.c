#include <stdio.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>

// Hello la BOP, c'est Gérard, avec cette focntion ,je cree un socket que je vais utilisé tout dans ce fichier network.c
{
    int socket_tcp;
    socket_tcp=socket(AF_INET, SOCK_STREAM,0);
    if (socket_tcp<0)
        {
            perror("La création du socket server a échouer");
            return -1;
        }
    return socket_tcp;
}
int init_server()
{
  return 0;
}

int main()
{
    int a;
    a=create_socket();
    printf("%d",a);
    return 0;
}
