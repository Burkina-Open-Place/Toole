#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <time.h>

typedef struct {
    char id[37];
    char username[64];
    char ip[16];
    int  port_tcp;
    char message[128];
    time_t last_time;
} device;

#endif
