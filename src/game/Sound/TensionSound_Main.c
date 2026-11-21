/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 NNS_SndPlayerStopSeq();
s32 NNS_SndHandleReleaseSeq();
s32 Task_Destroy();

void TensionSound_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u16 *)((u8 *)t0 + 0x4) = (*(s16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4) - 0x1);
    if (*(s16 *)((u8 *)t0 + 0x4) >= 0) {
        return;
    }
    NNS_SndPlayerStopSeq(*(u32 *)t0, 0x20);
    NNS_SndHandleReleaseSeq(*(u32 *)t0);
    Task_Destroy(*(u32 *)gTaskCurrent);
}
