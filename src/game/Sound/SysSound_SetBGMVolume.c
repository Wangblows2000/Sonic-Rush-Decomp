/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gSysSound[];
extern u8 gBGMVolume[];
s32 NNS_SndPlayerSetVolume();

void SysSound_SetBGMVolume(s32 p0) {
    s32 t0 = *(u32 *)gSysSound;
    if (t0 != 0) {
        s32 t1 = *(u8 *)gBGMVolume;
        if ((s32)p0 != t1) {
            *(u8 *)gBGMVolume = p0;
            NNS_SndPlayerSetVolume((t0 + 0x14), p0, gBGMVolume, t1);
        }
    }
}

#pragma thumb reset
