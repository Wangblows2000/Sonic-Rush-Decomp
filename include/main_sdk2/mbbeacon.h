#ifndef MAIN_SDK2_MBBEACON_H
#define MAIN_SDK2_MBBEACON_H
#include <nitro/types.h>

typedef struct {
    u32 ggid;         /* 0x00 */
    u8 lo : 2;        /* 0x04 */
    u8 hi : 6;
    u8 unk5;          /* 0x05 */
    u8 unk6;          /* 0x06 */
    u8 seq;           /* 0x07 */
    u16 sum;          /* 0x08 */
    u8 unkA;          /* 0x0a */
    u8 unkB;          /* 0x0b */
    u8 len;           /* 0x0c */
    u8 padD;
    u8 data[0x62];    /* 0x0e */
} UnkMbBeacon;

typedef struct {
    void *unk0;       /* 0x00 */
    u8 *obj;          /* 0x04 */
    u8 *cur;          /* 0x08 */
    u8 state;         /* 0x0c */
    u8 padD;
    u8 unkE;          /* 0x0e */
    u8 unkF;          /* 0x0f */
    u8 unk10;         /* 0x10 */
    u8 unk11;         /* 0x11 */
} UnkMbBeaconCtx;

extern UnkMbBeaconCtx data_02080e74;
extern UnkMbBeacon data_02080ea0;
extern u8 data_02080eae[];
#endif
