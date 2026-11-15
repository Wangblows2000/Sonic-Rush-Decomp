#ifndef MAIN_SDK2_WH_H
#define MAIN_SDK2_WH_H
#include <nitro/types.h>

typedef struct { u32 w[32]; } UnkGameInfo_wh; /* 0x80 bytes */

typedef struct {
    u16 apiid;          /* 0x00 */
    u16 errcode;        /* 0x02 */
    u16 state4;         /* 0x04 */
    u16 wlResult;       /* 0x06 */
    u16 state;          /* 0x08 */
    union {
        u8 mac[6];      /* 0x0a */
        u16 connAid;    /* 0x0a */
    };
    u16 aid;            /* 0x10 */
    u16 link;           /* 0x12 */
    u16 ssidLength;     /* 0x14 */
    u16 ssid[16];       /* 0x16 */
    u16 gameInfoLength; /* 0x36 */
    UnkGameInfo_wh gameInfo; /* 0x38 */
} UnkCallback_wh; /* 0xb8 */

typedef struct {
    u8 bssDesc[0xc0];
    UnkCallback_wh scan;
    u8 pad[8];
} UnkParentEntry_wh; /* 0x180 */

typedef struct {
    void *userGameInfo;         /* 0x00 */
    u16 userGameInfoLength;     /* 0x04 */
    u16 padding;                /* 0x06 */
    u32 ggid;                   /* 0x08 */
    u16 tgid;                   /* 0x0c */
    u16 entryFlag;              /* 0x0e */
    u16 maxEntry;               /* 0x10 */
    u16 multiBootFlag;          /* 0x12 */
    u16 KS_Flag;                /* 0x14 */
    u16 CS_Flag;                /* 0x16 */
    u16 beaconPeriod;           /* 0x18 */
    u16 rsv1[4];                /* 0x1a */
    u16 rsv2[8];                /* 0x22 */
    u16 channel;                /* 0x32 */
    u16 parentMaxSize;          /* 0x34 */
    u16 childMaxSize;           /* 0x36 */
    u16 rsv[4];                 /* 0x38 */
} UnkParentParam_wh;

typedef struct {
    u8 lo : 4;
    u8 aid : 4;
    u8 count;
    u16 data[10];
} UnkInitParam_wh; /* 0x16 */

typedef struct {
    UnkParentParam_wh parentParam; /* 0x000 */
    u8 sendBuf[0x400];          /* 0x040 */
    u8 scanBuf[0xc0];           /* 0x440 */
    u16 unk500;                 /* 0x500 */
    u16 unk502;                 /* 0x502 */
    void *recvBuf;              /* 0x504 */
    void (*unk508)(UnkCallback_wh *); /* 0x508 */
    u8 unk50c;                  /* 0x50c */
    u8 unk50d;                  /* 0x50d */
    u8 pad50e[0x518 - 0x50e];
    u16 sendBufSize;            /* 0x518 */
    u16 recvBufSize;            /* 0x51a */
    void (*callback)(u32, void *); /* 0x51c */
    u32 unk520;                 /* 0x520 */
    u16 unk524;                 /* 0x524 */
    u16 unk526;                 /* 0x526 */
    u16 unk528;                 /* 0x528 */
    u16 unk52a;                 /* 0x52a */
    u16 unk52c;                 /* 0x52c */
    u16 unk52e;                 /* 0x52e */
    u16 unk530[4];              /* 0x530 */
    u16 unk538[8];              /* 0x538 */
    u16 unk548;                 /* 0x548 */
    u8 pad54a[0x5e0 - 0x54a];
    u16 parentCount;            /* 0x5e0 */
    u16 aid;                    /* 0x5e2 */
    u32 unk5e4;                 /* 0x5e4 */
    u32 unk5e8;                 /* 0x5e8 */
    u32 unk5ec;                 /* 0x5ec */
    u8 pad5f0[0x10];
    UnkParentEntry_wh parents[16]; /* 0x600 */
} UnkWork_wh;

typedef struct {
    void *scanBuf;
    u16 channel;
    u16 maxChannelTime;
    u8 bssid[6];
} UnkScanParam_wh;

extern UnkWork_wh *data_02080f18;

typedef struct {
    u8 pad0[0x14];
    u32 ggid;  /* 0x14 */
    u8 maxEntry; /* 0x18 */
} UnkGroupInfo_wh;

typedef struct {
    u8 pad0[0xe4];
    u8 unkE4[0x5b8 - 0xe4];
    UnkGroupInfo_wh *info;      /* 0x5b8 */
    u8 pad5bc[4];
    u16 unk5c0;
    u16 unk5c2;
    u16 unk5c4;
    u16 unk5c6;
    u16 unk5c8;
    u8 unk5ca;
    u8 pad5cb[0x5d4 - 0x5cb];
} UnkEntry1788_wh;

typedef struct {
    u8 pad0[0x400];
    u8 recvBuf[0x1300 - 0x400]; /* 0x400 */
    UnkInitParam_wh initParam;  /* 0x1300 */
    u16 unk1316;                /* 0x1316 */
    u32 unk1318;                /* 0x1318 */
    u32 unk131c;                /* 0x131c */
    u32 unk1320;                /* 0x1320 */
    u8 pad1324[0x1340 - 0x1324];
    UnkInitParam_wh childInit[15]; /* 0x1340 */
    u16 unk148a[15];            /* 0x148a */
    u32 unk14a8[15];            /* 0x14a8 */
    void (*unk14e4)(u32, u32, u32); /* 0x14e4 */
    u32 unk14e8[15];            /* 0x14e8 */
    u8 unk1524;                 /* 0x1524 */
    u8 unk1525;                 /* 0x1525 */
    s8 unk1526[15];             /* 0x1526 */
    u8 unk1535;                 /* 0x1535 */
    u16 unk1536;                /* 0x1536 */
    u8 unk1538[0x1754 - 0x1538]; /* 0x1538 */
    u16 unk1754[15];            /* 0x1754 */
    u8 pad1772[0x1788 - 0x1772];
    UnkEntry1788_wh unk1788[16]; /* 0x1788 */
    u32 unk74c8;                /* 0x74c8 */
} UnkStruct_02080f1c;
extern UnkStruct_02080f1c *data_02080f1c;

#endif
