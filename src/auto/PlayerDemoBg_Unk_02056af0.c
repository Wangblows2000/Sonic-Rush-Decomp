/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void PlayerDemoBg_Unk_02056a84();
s32 Task_Create();

void PlayerDemoBg_Unk_02056af0(s32 p0) {
    s32 t0 = Task_Create((u32)PlayerDemoBg_Unk_02056a84, 0, 0, 0xf000, 3, 4);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
}

#pragma thumb reset
