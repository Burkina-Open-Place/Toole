#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>


/*
Dans cette partie , je vais creé des helpers pour evité de repeter la logique de lecture
ou d'ecriture des fichiers ou meme  eviter d'avoir des fichiers corrompus
*/

static int write_n(int socket_tcp,const void *buffer,size_t n){
    const uint8_t *p=(const uint8_t *)buffer;
    size_t sent=0;
    while (sent<n) {
        ssize_t w=send(socket_tcp,p+sent,n-sent,0);
        if(w<0)return -1;
        if (w==0) return -1;
        sent+=(size_t)w;
    }
    return 0;
}

//on convertit l'ordre des octets avant de les envoyés pour optimiser le compatibilité entre architecture little-endian et big-endian
// les octets d’un entier multi-octets (ex: uint64_t) sont stockés dans l’ordre inverse
// sans ca le recepteur pourrait reconstruire une valeur incorrecte à la reception
static uint64_t htonll(uint64_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ((uint64_t)htonl((uint32_t)(x & 0xFFFFFFFFULL)) << 32) |
            htonl((uint32_t)(x >> 32));
#else
    return x;
#endif


/* Là maintenant , c'est la logique de transfere du fichier qui servira à envoyer et à recevoir les fichiers */
typedef struct {
    uint32_t name_len;
    uint64_t file_size;
    } file_struct;

// Dans cette fonction, j'envoie une structure avec les infos des fichiers qui permettra de les reassemblé apres envoie
int send_struct(int socket_tcp,const char *filename,uint64_t file_size){
    if(!filename)return -1;
    size_t name_len_size=strlen(filename);
    if(name_len_size==0 ||name_len_size< UINT32_MAX) return -1;

    file_struct one;
    one.name_len  = htonl((uint32_t)name_len_size);
    one.file_size = htonll(file_size);

    //let's go envoyons l'en tete
    if(write_n)
}
//Hello la BOP, encore Gérard sur ce nouveau fichier , je cree cette focntion pour l'envoie de fichier à transfere un socket TCP dejà existant
int send_file(){

}

// là cette focntion est l'equivalent de send_struct à la reception
int recv_struct(){

}

// ici, c'est la focntion qui permettra de recevoir le fichier envoyé
int recv_file(){

}

int main(){
    return 0;
}
