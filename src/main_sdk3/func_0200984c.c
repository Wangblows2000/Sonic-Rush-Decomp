#include <nitro/types.h>

typedef struct {
    s16 localVariable[16];
    u32 tickCounter;
} UnkSndPlayerShared_0200984c;

typedef struct {
    u32 finishCommandTag;          /* 0x00 */
    u32 playerStatus;              /* 0x04 */
    u16 channelStatus;             /* 0x08 */
    u16 captureStatus;             /* 0x0a */
    u8 padc[0x14];
    UnkSndPlayerShared_0200984c player[16];   /* 0x20 */
    s16 globalVariable[16];        /* 0x260 */
} UnkSndSharedWork_0200984c;

extern void DC_FlushRange(const void *p, u32 size);

void func_0200984c(UnkSndSharedWork_0200984c *work)
{
    s32 i;
    s32 j;
    work->playerStatus = 0;
    work->channelStatus = 0;
    work->captureStatus = 0;
    work->finishCommandTag = 0;
    for (i = 0; i < 16; i++) {
        work->player[i].tickCounter = 0;
        for (j = 0; j < 16; j++) {
            work->player[i].localVariable[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        work->globalVariable[i] = -1;
    }
    DC_FlushRange(work, sizeof(UnkSndSharedWork_0200984c));
}
