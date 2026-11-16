/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 TalkDemo_GetScriptHeader();
s32 TalkDemo_GetFlags();
extern u8 data_02076c54[];
s32 Sound_PlaySeq();

void TalkDemo_PlayBGM(u8 *p0) {
    s32 t0 = TalkDemo_GetScriptHeader(*(u32 *)((u8 *)p0 + 0x24));
    s32 t1 = ((s32)(*(u32 *)((u8 *)t0 + 0x18) << 0x10) >> 0x10);
    s32 t2 = TalkDemo_GetFlags(0);
    if ((t2 & 0x10) == 0) {
        if (t1 >= 0) {
            if (t1 < 0xa) {
                if (t1 != 0) {
                    if (t1 != 1) {
                        if (t1 != 2) {
                            Sound_PlaySeq(((u8 *)p0 + 0x2c), *(s16 *)((u8 *)data_02076c54 + (t1 << 1)));
                        }
                    }
                }
            }
        }
    }
}

#pragma thumb reset
