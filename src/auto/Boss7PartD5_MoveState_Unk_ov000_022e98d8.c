/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss7[];
s32 Boss7_Unk_ov000_022ee198();
s32 Boss7_Unk_ov000_022e9b70();
s32 func_ov000_02326f20();
extern u8 gBossStatus[];
void Boss7PartD5_MoveState_Unk_ov000_022e97cc();

void Boss7PartD5_MoveState_Unk_ov000_022e98d8(u8 *p0) {
    s32 t0 = *(u32 *)gBoss7;
    *(u32 *)((u8 *)t0 + 0x210) = 1;
    Boss7_Unk_ov000_022ee198(p0, 0x3c, 1);
    Boss7_Unk_ov000_022e9b70(p0, 1, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x480);
    s32 v2 = t1;
    if (t1 != 0) {
        func_ov000_02326f20(v2);
        *(u32 *)((u8 *)p0 + 0x480) = 0;
        v2 = 0;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x488);
    if (t3 != 0) {
        *(u32 *)((u8 *)t3 + 0x54) = (*(u32 *)((u8 *)t3 + 0x54) | 8);
        *(u32 *)((u8 *)p0 + 0x488) = 0;
    }
    *(u32 *)((u8 *)t0 + 0x20c) = (*(u32 *)((u8 *)t0 + 0x20c) & ~0x10);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x24;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)Boss7PartD5_MoveState_Unk_ov000_022e97cc;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x440))(p0, *(u32 *)((u8 *)p0 + 0x440), 0x24);
}
