#pragma opt_propagation off
#include <nitro/types.h>

typedef struct {
    BOOL activeFlag : 1;
    BOOL lockFlag : 1;
    u16 volume;      /* 0x04 */
    u8 pan;          /* 0x06 */
    u8 pad7;
    u32 envStatus;   /* 0x08 */
} UnkChannelInfo_02009900;

typedef struct {
    u8 pad0[2];
    u8 envStatus;    /* 0x02 */
    u8 active : 1;   /* 0x03 */
    u8 pad4[0x23 - 4];
    u8 pan;          /* 0x23 */
    u16 volume;      /* 0x24 */
    u8 pad26[0x54 - 0x26];
} UnkChannel_02009900;

typedef struct {
    UnkChannel_02009900 channel[16];     /* 0x000 */
    u8 pad540[0x1180 - 0x540];
    u32 lockedReg[16];                   /* 0x1180 */
    u8 pad11c0[4];
    u32 lockMask;                        /* 0x11c4 */
} UnkSndWork_02009900;

BOOL SND_ReadChannelInfo(const UnkSndWork_02009900 *work, s32 ch, UnkChannelInfo_02009900 *info)
{
    if (ch < 0 || ch > 15) {
        return FALSE;
    }
    info->lockFlag = (work->lockMask & (1 << ch)) ? TRUE : FALSE;
    if (info->lockFlag) {
        u32 reg = work->lockedReg[ch];
        s32 div;
        info->activeFlag = (reg & 0x80000000) ? TRUE : FALSE;
        div = (reg & 0x300) >> 8;
        info->volume = (u8)(reg & 0x7f);
        info->volume <<= 4;
        switch (div) {
        case 0:
            break;
        case 1:
            info->volume >>= 1;
            break;
        case 2:
            info->volume >>= 2;
            break;
        case 3:
            info->volume >>= 4;
            break;
        }
        info->pan = (u8)((reg & 0x7f0000) >> 16);
    } else {
        const UnkChannel_02009900 *chp = &work->channel[ch];
        u16 vol;
        s32 div;
        info->activeFlag = chp->active;
        info->envStatus = chp->envStatus;
        vol = chp->volume;
        div = vol >> 8;
        info->volume = (u8)vol;
        info->volume <<= 4;
        switch (div) {
        case 0:
            break;
        case 1:
            info->volume >>= 1;
            break;
        case 2:
            info->volume >>= 2;
            break;
        case 3:
            info->volume >>= 4;
            break;
        }
        info->pan = chp->pan;
    }
    return TRUE;
}
