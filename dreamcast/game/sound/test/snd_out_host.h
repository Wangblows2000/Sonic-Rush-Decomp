#ifndef SND_OUT_HOST_H
#define SND_OUT_HOST_H
#include <stdint.h>
#define HOST_OUT_MAX 65536
typedef struct { int type, ch; uint32_t aram, freq; int vol, pan; unsigned what; } HostOutEvent; /* type 1 start 2 update 3 stop */
typedef struct {
    HostOutEvent ev[HOST_OUT_MAX];
    int n, dropped;
    uint32_t starts, updates, stops, uploaded;
    int on[16];
} HostOutLog;
extern HostOutLog g_host_out;
#endif
