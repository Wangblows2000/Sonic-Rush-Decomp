/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 NNS_SndPlayerStopSeq();
s32 NNS_SndHandleReleaseSeq();
s32 Sound_FreeHandle();

void func_ov000_0232ec74(u8 *p0) {
    if (*(u32 *)*(u32 *)((u8 *)p0 + 0x10) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    NNS_SndPlayerStopSeq(*(u32 *)*(u32 *)((u8 *)p0 + 0x10), 0);
    NNS_SndHandleReleaseSeq(*(u32 *)t0);
    Sound_FreeHandle(*(u32 *)t0);
}
