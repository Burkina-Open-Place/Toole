#include "discovery.h"


//Hello le BOP , ici dans ce fichier ,je creér des fonctions pour gérer le logique
//concurente  de nos differentes composantes

//cette type de fonction de fonction de sont celle de hear() et de presence dans discovery.c
// elles seront utilisé par la suite pour permettre à discovery_multiplex() de prendre les focntions hear et presence en parametre
typedef int (*presence_fn)(
    int socket_udp,
    const char *id,
    const char *username,
    const char *ip,
    int port_tcp,
    const char *message
);

typedef void (*hear_fn)(
    int socket_udp,
    device *liste,
    int *nb
);
