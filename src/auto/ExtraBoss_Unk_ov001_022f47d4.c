/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();
void ExtraBossObj_State_Unk_ov001_022f4634();
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();
s32 ExtraBoss_GetPlayer();
s32 ExtraBoss_Unk_ov001_02303abc();
s32 ExtraBoss_Unk_ov001_02303a80();

void ExtraBoss_Unk_ov001_022f47d4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1dc);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x84);
    s32 t2 = ExtraBoss_GetWork();
    *(u32 *)((u8 *)t0 + 0x8) = *(u32 *)t0;
    *(u32 *)t0 = 3;
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_Unk_ov001_022f4634;
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) | 0x40);
    Model_SetAnimation(t1, 0, *(u32 *)data_ov001_0231f068, 2, 0);
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) & ~4);
    *(u16 *)((u8 *)(t1 + 0x100) + 0x2c) = 0x400;
    *(u32 *)((u8 *)t1 + 0x114) = 0x1000;
    *(u32 *)((u8 *)p0 + 0x50) = 0x4000;
    *(u32 *)((u8 *)p0 + 0x5c) = 0xe0;
    *(u32 *)((u8 *)p0 + 0x60) = 0;
    *(u32 *)((u8 *)p0 + 0x54) = *(u32 *)((u8 *)p0 + 0x60);
    *(u32 *)((u8 *)p0 + 0xb8) = (*(u32 *)((u8 *)p0 + 0xb8) | 0x200);
    s32 t3 = (*(u32 *)((u8 *)p0 + 0xf0) | 0x200);
    *(u32 *)((u8 *)p0 + 0xf0) = t3;
    *(u16 *)((u8 *)p0 + 0x1c8) = 0;
    *(u16 *)((u8 *)p0 + 0x1cc) = 0;
    *(u32 *)((u8 *)t2 + 0x318) = 1;
    s32 t4 = ExtraBoss_GetPlayer(((u8 *)p0 + 0x100), 1, t3, 0);
    s32 t5 = *(u32 *)((u8 *)t4 + 0x1dc);
    *(u32 *)((u8 *)t4 + 0xb8) = (*(u32 *)((u8 *)t4 + 0xb8) | 0x200);
    *(u32 *)((u8 *)t4 + 0xf0) = (*(u32 *)((u8 *)t4 + 0xf0) | 0x200);
    s32 t6 = (*(u32 *)((u8 *)t4 + 0x14) & ~0x20000);
    *(u32 *)((u8 *)t4 + 0x14) = t6;
    s32 t7 = (*(u32 *)t5 & ~2);
    *(u32 *)t5 = t7;
    *(u32 *)((u8 *)t5 + 0x10) = 0;
    s32 t8 = ExtraBoss_GetWork(t7, 0, t6, t5);
    s32 t9 = (*(u32 *)t8 | 2);
    *(u32 *)t8 = t9;
    ExtraBoss_Unk_ov001_02303abc(0, t9);
    ExtraBoss_Unk_ov001_02303a80(0);
}
