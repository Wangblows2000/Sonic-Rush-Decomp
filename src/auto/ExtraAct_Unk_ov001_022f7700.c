/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f084[];
extern u8 gTaskCurrent[];
void ExtraAct_State_Unk_ov001_022f75c8();

void ExtraAct_Unk_ov001_022f7700(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe9c) - 0x1);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe9c) = t0;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe9c);
    if (t1 <= 0) {
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ExtraAct_State_Unk_ov001_022f75c8;
    }
}
