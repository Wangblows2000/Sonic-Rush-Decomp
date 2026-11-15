#ifndef MAIN_SDK2_WMDS_H
#define MAIN_SDK2_WMDS_H
#include <nitro/types.h>

typedef struct {
    u16 bitmap0;
    u16 bitmap1;
    u8 data[0x1fc];
} UnkDSBuf;

typedef struct {
    UnkDSBuf buf[4];  /* 0x000 */
    u16 seq[4];       /* 0x800 */
    u16 writeIdx;     /* 0x808 */
    u16 readIdx;      /* 0x80a */
    u16 unk80c;       /* 0x80c */
    u16 aidBitmap;    /* 0x80e */
    u16 dataLength;   /* 0x810 */
    u16 count;        /* 0x812 */
    u16 setLength;    /* 0x814 */
    u16 port;         /* 0x816 */
    u16 doubleMode;   /* 0x818 */
    u16 unk81a;       /* 0x81a */
    u16 state;        /* 0x81c */
    u16 unk81e;       /* 0x81e */
} UnkDS;

void *func_02011f4c(void *a, u16 bitmap, void *data, u16 aid);
#endif
