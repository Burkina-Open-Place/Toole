#include <poll.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "discovery.h"
#include "server_runtime.h"

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

// cette fonction prend des repères(timespec) dans le temps et renvoit la dureé ecoulé entre ses deux reperes
static int duration(struct timespec a, struct timespec b)
{
    long sec = b.tv_sec - a.tv_sec;
    long nsec = b.tv_nsec - a.tv_nsec;
    if (nsec < 0)
    {
        sec--;
        nsec += 1000000000L;
    }
    int d=(sec * 1000 + nsec / 1000000);
    return d;
}
