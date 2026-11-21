/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f084[];
void ExtraAct_Unk_ov001_022f904c();
s32 Task_Create();

void ExtraAct_Unk_ov001_022f9134(void) {
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe44) = 0x3c;
    s32 t0 = Task_Create((u32)ExtraAct_Unk_ov001_022f904c, 0, 0, 0x2000, 0, 0);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe0c) = t0;
}
