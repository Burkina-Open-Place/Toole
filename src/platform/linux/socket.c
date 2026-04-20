#include <asm-generic/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#define BEACON_PORT 47272

// Presence est la focntion qui emet les beacon
int presence(char *username,char *ip,int port_tcp,char *message)
{
    char beacon[256];
    snprintf(beacon,sizeof(beacon), "toole|%s|%s|%d|%s",username,ip,port_tcp,message);

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
    //Cette structure definit les adresse et port reseau pour entamer  l'emission de données en UDP
    struct sockaddr_in network_utils={
        .sin_family= AF_INET,
        .sin_port= htons(BEACON_PORT),
        .sin_addr.s_addr= inet_addr("255.255.255.255")
    };
    // Envoie du beacon de presence avec l'ip , le port et le message du TCP
    sendto(socket_udp,beacon,strlen(beacon),0,(struct sockaddr *)&network_utils,sizeof(network_utils));
    close(socket_udp);
    return 0;
}
//-------------------------------------------------------------------------
//hear ecoute les beacon sur le port d'emmision
int hear(){

    //Hello le BOP ici je cree un socket UDP pour la fonction hear()
    int socket_udp;
    socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_udp<0){
        perror("La creation du socket a echoué");
        return -1;
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
        return -1;
    }
    //cette structure permet de mettre un timeout de 2 second
    struct timeval timeout =
        {
            .tv_sec = 2,
            .tv_usec = 0
        };

    setsockopt(socket_udp, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char buffer[256];
    socklen_t size_of=sizeof(network_utils);

    if(recvfrom(socket_udp, buffer, sizeof(buffer)-1, 0,(struct sockaddr *)&network_utils, &size_of)<0){
        perror("Erreur de reception");
    }
    printf("%s",buffer);
    return 0;
}

int main(void)
{
    while (1) {
    presence("Gérard","192.168.100.1",47222,"auto");
    hear();
    sleep(1);
    }
    hear();
    return 0;
}
