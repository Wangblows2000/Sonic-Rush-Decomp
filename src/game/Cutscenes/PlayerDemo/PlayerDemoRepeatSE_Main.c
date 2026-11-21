/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 PlayerDemo_IsActive();
s32 Task_Destroy();
s32 NNS_SndPlayerStopSeq();
s32 NNS_SndHandleReleaseSeq();
s32 Sound_PlaySeqArc();

void PlayerDemoRepeatSE_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = PlayerDemo_IsActive(*(u32 *)gTaskCurrent);
    if (t1 == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    *(u8 *)((u8 *)t0 + 0x9) = (*(s8 *)((u8 *)t0 + 9) - 0x1);
    if (*(s8 *)((u8 *)t0 + 9) <= 0) {
        *(u8 *)((u8 *)t0 + 0x9) = *(u8 *)((u8 *)t0 + 0x8);
        s32 t2 = *(u32 *)((u8 *)t0 + 0x4);
        if (*(u32 *)*(u32 *)((u8 *)t0 + 0x4) != 0) {
            NNS_SndPlayerStopSeq(t2, 0);
            NNS_SndHandleReleaseSeq(*(u32 *)((u8 *)t0 + 0x4));
        }
        Sound_PlaySeqArc(*(u32 *)((u8 *)t0 + 0x4), *(u8 *)((u8 *)t0 + 0xa), *(u8 *)((u8 *)t0 + 0xb));
    }
}

#pragma thumb reset
