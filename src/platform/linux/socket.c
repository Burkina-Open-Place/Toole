#include <asm-generic/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define BEACON_PORT 47272

int presence_socket()
{
    //Hello le BOP,sur cette section , je creer le socket qui retourne un nombre negatif si echec et  un nombre positif si succès
    int socket_udp;
    socket_udp=socket(AF_INET, SOCK_DGRAM,0);
    if (socket_udp < 0)
    {
        perror("La creation du socket du socket a echoué");
        return -1;
    }


    //la fonction setsockopt() de <sys/socket.h> sera utilisé pour preparer le broadcast , c'est elle qui definit le fonctionement du socket
    int enable= 1;
    if(setsockopt(socket_udp, SOL_SOCKET,SO_BROADCAST,&enable,sizeof(enable))<0)
    {
        perror("setsockopt a echoué");
        return -1;
    }
    return socket_udp;
}

// Presence est la focntion qui emet les beacon
int presence(int socket_udp,char *id,char *username,char *ip,int port_tcp,char *message)
{
    char beacon[256];
    snprintf(beacon,sizeof(beacon), "toole|%s|%s|%s|%d|%s",id,username,ip,port_tcp,message);

    //Cette structure definit les adresse et port reseau pour entamer  l'emission de données en UDP
    struct sockaddr_in network_utils={
        .sin_family= AF_INET,
        .sin_port= htons(BEACON_PORT),
        .sin_addr.s_addr= inet_addr("255.255.255.255")
    };
    // Envoie du beacon de presence avec l'ip , le port et le message du TCP
    sendto(socket_udp,beacon,strlen(beacon),0,(struct sockaddr *)&network_utils,sizeof(network_utils));
    return 0;
}
//-------------------------------------------------------------------------
typedef struct {
    char id[37];
    char username[64];
    char ip[16];
    int  port_tcp;
    char message[128];
} device;
int nb=0;
//hear ecoute les beacon sur le port d'emmision
device *hear(void)
{

    //Hello le BOP ici je cree un socket UDP pour la fonction hear()
    int socket_udp;
    socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_udp<0){
        perror("La creation du socket a echoué");
    }

    // creation de la stucture(support pour la transmision)
    struct sockaddr_in network_utils=
        {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family= AF_INET,
        .sin_port= htons(BEACON_PORT)
    };

    //ici j'attache le socket à la structure en haut
    if(bind (socket_udp, (struct sockaddr *) &network_utils, sizeof( network_utils))< 0)
    {
        perror("bind() a echoué");
        close(socket_udp);
        return NULL;
    }


    int capacite = 4;
    int count = 0;
    device *liste = malloc(capacite * sizeof(device));
    if (liste == NULL) {
        perror("malloc a echoue");
        close(socket_udp);
        return NULL;
    }

    char buffer[256];
    socklen_t size_of=sizeof(network_utils);

    while (1) {
        ssize_t result=recvfrom(socket_udp, buffer, sizeof(buffer)-1, 0,(struct sockaddr *)&network_utils, &size_of);

        if(result<0) break;
        buffer[result]='\0';

        if (strncmp(buffer, "toole", 5) != 0) continue;
        if (count == capacite) {
            capacite *= 2;
            device *tmp = realloc(liste, capacite * sizeof(device));
            if (tmp == NULL) {
                perror("hear() -> realloc a echoue");
                break;
            }
            liste = tmp;
                }
        device d;
        sscanf(buffer, "toole|%36[^|]|%63[^|]|%15[^|]|%d|%127[^\n]",d.id, d.username, d.ip, &d.port_tcp, d.message);
        liste[count] = d;
        count++;
        }
        close(socket_udp);
        nb = count;

        return liste;
    }


int main(void)
{
    int sock=presence_socket();
    while (1) {
    presence(sock, "T-001", "Gerard", "192.168.100.1", 42422,"auto");
    sleep(5);
    }
    close(sock);
    return 0;
}
