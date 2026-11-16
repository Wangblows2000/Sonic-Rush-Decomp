/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 NNS_SndPlayerStopSeq();
s32 NNS_SndHandleReleaseSeq();
extern u8 data_02075a34[];
s32 Sound_PlaySeq();

void func_02057d54(u8 *p0, s32 p1) {
    if ((*(u32 *)((u8 *)p0 + 0x4) & 0x10000000) == 0) {
        if ((s32)p1 >= 0) {
            if ((s32)p1 < 0xc) {
                if (*(u32 *)((u8 *)p0 + 0x30) != 0) {
                    NNS_SndPlayerStopSeq(((u8 *)p0 + 0x30), 0);
                    NNS_SndHandleReleaseSeq(((u8 *)p0 + 0x30));
                }
                Sound_PlaySeq(((u8 *)p0 + 0x30), *(s16 *)((u8 *)data_02075a34 + ((s32)p1 << 1)));
            }
        }
    }
}

#pragma thumb reset
