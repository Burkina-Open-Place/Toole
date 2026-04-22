
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib,"Ws2_32.lib")//pour indiquer au compilateur d'ajouter la bib ws2_32.lib lors de la compilation
#define PORT 47272
#define BROADCAST_IP "255.255.255.255"//address de diffusion


// En gros ,cette fonction ci-dessous permet la 1=>de signaler sa presence sur le resau 2=>detecter la presence des autres pc.
int presence(char *ip,int port_tcp,char *message); 

//generer des id pour les pc.
char * id_generertor();
/*fonction main:l'entre du code | programe*/
int main(void)
{
    presence("172.16.17.140",PORT,"gosse");
    char *name_pc = id_generertor();
    printf("%s",name_pc);
    free(name_pc);
    return 0;
}

/*Implementation des fonction */
int presence(char *ip,int port_tcp,char *message){

    char beacon[256];
    snprintf(beacon,sizeof(beacon), "toole|%s|%d|%s",ip,port_tcp,message);
    WSADATA Sa;
    if(WSAStartup(MAKEWORD(2,2),&Sa) != 0){
        printf("initialisation de la socket a échoué : %d", WSAGetLastError());
        exit(1);
    }
    SOCKET sock_envoi = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_envoi == INVALID_SOCKET){
        printf("creation de la socket  a échoué : %d", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    //bro windows a par defaut bloque le broadcast donc cette petite manipe nous permet de debloque le broadcast sur windows
    BOOL broadcastPermission = TRUE;
    if(setsockopt(sock_envoi,SOL_SOCKET,SO_BROADCAST,(char*)&broadcastPermission,sizeof(broadcastPermission)) == SOCKET_ERROR){
        printf("permition de broadcast  a échoué : %d", WSAGetLastError());
        closesocket(sock_envoi);
        WSACleanup();
        exit(1);
    }
    u_long mode = 1;
    if(ioctlsocket(sock_envoi,FIONBIO,&mode) == SOCKET_ERROR){
        printf("erreur d'ecoute: %d",WSAGetLastError());
        exit(1);
    }

    struct sockaddr_in infosock_sa;
    infosock_sa.sin_family = AF_INET;
    infosock_sa.sin_port = htons(PORT);
    infosock_sa.sin_addr.s_addr = INADDR_BROADCAST;


    if (sendto(sock_envoi, beacon, strlen(beacon), 0, (struct sockaddr *)&infosock_sa, sizeof(infosock_sa)) == SOCKET_ERROR) {
            printf("sendto() a échoué : %d", WSAGetLastError());

    }
        

    // comme tu le sais : si tu a ouvert forcer tu vas  Fermeture.ainsi cette fonction permet de liberer les resource.
    closesocket(sock_envoi);
    WSACleanup();
    return 0;
    }

char * id_generertor(){
    int nombre;
    char chaine[10];
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    char *namepc = malloc(MAX_COMPUTERNAME_LENGTH * sizeof(char));
    if(namepc == NULL){
        fprintf(stderr,"erreur de l'allocstion de namepc");
        return "INCONNU";
    }

    //Avec cette fonction{GetComputerName} je recupere le nom de ta machine et avec {strcpy} je copie le nom de tas machine dans la variable computerName
    if (GetComputerName(computerName,&size))
    {
        strcpy(namepc,computerName);
        
    }else {
        printf("Erreur lors de la recuperation du nom (code : %lu)\n", GetLastError());
        return "INCONNU";
    }
    srand(time(NULL));
    nombre = rand() % 1001;//generation d'un nombre alleatoire pour faire l'id
    sprintf(chaine, "%d", nombre);
        // 1. Copier computerName dans namepc
    strcpy(namepc, computerName);                
    
    // 2. Concaténer chaine à la suite de namepc
    strcat(namepc, chaine);  
    
    return namepc;//je retourne l'id avec ce format {NOMPCNOMBRE} EX:DELL57665555 NB:5555 est le nombre generer et DELL5766 nom de la machine.
}