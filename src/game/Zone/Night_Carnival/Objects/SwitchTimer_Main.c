/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
extern u8 gSwitchTimers[];
s32 Task_Destroy();
extern u8 gPlayers[];
s32 NNS_SndPlayerSetTempoRatio();

void SwitchTimer_Main(void) {
    if (*(s16 *)((u8 *)gSwitchTimers + (*(s16 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) << 1)) == 0x3fc) {
        return;
    }
    s32 t0 = *(u32 *)gTaskCurrent;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t2 = *(s16 *)((u8 *)gSwitchTimers + (*(s16 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) << 1));
    s32 t3 = (*(s16 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) << 1);
    if (t2 != 0) {
        s32 t4 = (*(s16 *)((u8 *)gSwitchTimers + t3) - 0x1);
        *(u16 *)((u8 *)gSwitchTimers + t3) = t4;
    } else {
        Task_Destroy(t0, t2, gSwitchTimers, t3);
        return;
    }
    s32 t5 = (*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400);
    if (t5 != 0) {
        *(u16 *)((u8 *)t1 + 0x6) = 1;
    }
    if (*(u16 *)((u8 *)t1 + 0x6) != 0) {
        if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400) == 0) {
            Task_Destroy(*(u32 *)gTaskCurrent);
            return;
        }
    }
    if (*(s16 *)((u8 *)gSwitchTimers + (*(s16 *)t1 << 1)) < 0x78) {
        s32 t6 = *(u16 *)((u8 *)t1 + 0x4);
        if (t6 == 0) {
            *(u16 *)((u8 *)t1 + 0x4) = 1;
        }
    }
    if (*(u16 *)((u8 *)t1 + 0x4) == 0) {
        return;
    }
    s32 t7 = *(u16 *)((u8 *)t1 + 0x4);
    s32 t8 = *(s16 *)((u8 *)gSwitchTimers + (*(s16 *)t1 << 1));
    if (t8 == 0x3c) {
        NNS_SndPlayerSetTempoRatio(*(u32 *)((u8 *)t1 + 0x8), 0x200, t7);
        *(u16 *)((u8 *)t1 + 0x4) = 2;
        return;
    }
    if (t8 <= 0x3c) {
        return;
    }
    if (t7 != 2) {
        return;
    }
    NNS_SndPlayerSetTempoRatio(*(u32 *)((u8 *)t1 + 0x8), 0x100);
    *(u16 *)((u8 *)t1 + 0x4) = 1;
}
