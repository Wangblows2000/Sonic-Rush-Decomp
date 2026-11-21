#ifndef MAIN_SDK2_SNDP_H
#define MAIN_SDK2_SNDP_H
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u8 volume; /* 0x20 */
} UnkSndInfo;

typedef struct {
    u8 data[0x10];
} UnkSndFader;

struct UnkSndHandle;

typedef struct UnkSndPlayer {
    struct UnkSndHandle *handle; /* 0x00 */
    UnkSndInfo *info;            /* 0x04 */
    u8 pad8[0x1c - 0x08];
    UnkSndFader fader;           /* 0x1c */
    u8 status;                   /* 0x2c */
    u8 prepared;                 /* 0x2d */
    u8 unk2e;                    /* 0x2e */
    u8 pauseReq;                 /* 0x2f */
    u32 seqId;                   /* 0x30 */
    u8 pad34[0x3c - 0x34];
    u8 playerNo;                 /* 0x3c */
    u8 prio;                     /* 0x3d */
    s16 volume;                  /* 0x3e */
    u8 volume1;                  /* 0x40 */
    u8 volume2;                  /* 0x41 */
} UnkSndPlayer;

typedef struct UnkSndHandle {
    UnkSndPlayer *player;
} UnkSndHandle;

typedef struct {
    u8 list[8];     /* 0x00 NNSFndList-like */
    u16 count;      /* 0x08 */
    u8 pad[0x18 - 0x0a];
    u32 max;        /* 0x18 */
    u8 pad1c[0x24 - 0x1c];
} UnkSndPlayerHeap;

void *NNS_FndGetNextListObject(void *list, void *obj);
#endif
