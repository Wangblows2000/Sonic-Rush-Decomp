/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss4[];
extern u8 gBossStatus[];
s32 Boss4_Unk_ov000_022dccc8();
extern u8 data_ov000_02363580[];
s32 Boss4_Unk_ov000_022dcd80();
s32 Boss4_Unk_ov000_022dc8cc();

void Boss4_State_Unk_ov000_022dea0c(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)gBoss4;
    *(u32 *)((u8 *)t0 + 0x218) = (*(u32 *)((u8 *)*(u32 *)gBoss4 + 0x218) + 0x1);
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x224) + 0x1);
    *(u32 *)((u8 *)t0 + 0x224) = t1;
    if ((u32)(*(u32 *)((u8 *)gBossStatus + 0x4) - 0x18) <= (u32)1) {
        Boss4_Unk_ov000_022dccc8(t0, t1);
        *(u32 *)((u8 *)t0 + 0xbe8) = *(s16 *)((u8 *)data_ov000_02363580 + ((*(u16 *)((u8 *)gBossStatus + 0x8) - 0x1) << 1));
        *(u32 *)((u8 *)t0 + 0xbe4) = 0;
        *(u32 *)((u8 *)t0 + 0xbe0) = 0;
    } else {
        Boss4_Unk_ov000_022dcd80(t0, t1, p2);
        Boss4_Unk_ov000_022dc8cc(t0);
    }
    if (*(u32 *)((u8 *)t0 + 0x220) == 0) {
        return;
    }
    ((s32 (*)())*(u32 *)((u8 *)t0 + 0x220))(t0, *(u32 *)((u8 *)t0 + 0x220));
}
