/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ExtraAct_RankStampDelay();
s32 Task_Create();
extern u8 data_ov001_0231f084[];

void ExtraAct_StartRankStamp(void) {
    s32 t0 = Task_Create((u32)ExtraAct_RankStampDelay, 0, 0, 0x1e00, 0, 0);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe20) = t0;
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe9c) = 0x3c;
}
