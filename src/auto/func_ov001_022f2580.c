/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022eec34();
void func_ov001_022f240c();
extern u8 data_ov001_02316840[];
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();
extern u8 data_ov001_0231f074[];
s32 ExtraBoss_GetWork();
s32 func_02002094();

void func_ov001_022f2580(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 > 0) {
        func_ov001_022eec34(p0, t0);
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x84);
    s32 t2 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18);
    s32 t3 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18) + 0x84);
    *(u32 *)((u8 *)p0 + 0x60) = 0;
    *(u32 *)((u8 *)p0 + 0x5c) = *(u32 *)((u8 *)p0 + 0x60);
    *(u32 *)((u8 *)p0 + 0x54) = *(u32 *)((u8 *)p0 + 0x5c);
    *(u32 *)((u8 *)p0 + 0x50) = *(u32 *)((u8 *)p0 + 0x54);
    *(u32 *)((u8 *)p0 + 0x74) = (u32)func_ov001_022f240c;
    *(u32 *)((u8 *)p0 + 0xb8) = (*(u32 *)((u8 *)p0 + 0xb8) | 0x200);
    *(u32 *)((u8 *)p0 + 0x128) = (*(u32 *)((u8 *)p0 + 0x128) & ~0x200);
    *(u32 *)((u8 *)t2 + 0xf0) = (*(u32 *)((u8 *)t2 + 0xf0) & ~0x200);
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) | 0x40);
    Model_SetAnimation(t1, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0x28), 0);
    Model_SetAnimation(t1, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0x2a), 0);
    *(u16 *)((u8 *)(t1 + 0x100) + 0x2c) = 0x400;
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) & ~4);
    s32 t4 = (*(u8 *)((u8 *)t1 + 0x110) & ~4);
    *(u8 *)((u8 *)t1 + 0x110) = t4;
    s32 t5 = ExtraBoss_GetWork(t4, 0x400);
    s32 t6 = ExtraBoss_GetWork();
    s32 t7 = ExtraBoss_GetWork();
    s32 t8 = ExtraBoss_GetWork();
    s32 t9 = func_02002094(((*(u32 *)((u8 *)t5 + 0x308) * 0xccc) + ((*(u32 *)((u8 *)t6 + 0x30c) - *(u32 *)((u8 *)t7 + 0x308)) << 0xc)), *(u32 *)((u8 *)t8 + 0x30c), (*(u32 *)((u8 *)t6 + 0x30c) - *(u32 *)((u8 *)t7 + 0x308)), *(u32 *)((u8 *)t5 + 0x308));
    *(u32 *)((u8 *)t1 + 0x114) = t9;
    *(u8 *)((u8 *)t3 + 0x10c) = (*(u8 *)((u8 *)t3 + 0x10c) | 0x40);
    Model_SetAnimation(t3, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0x2c), 0);
    Model_SetAnimation(t3, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0x2e), 0);
    *(u16 *)((u8 *)(t3 + 0x100) + 0x2c) = 0x400;
    *(u8 *)((u8 *)t3 + 0x10c) = (*(u8 *)((u8 *)t3 + 0x10c) & ~4);
    *(u8 *)((u8 *)t3 + 0x110) = (*(u8 *)((u8 *)t3 + 0x110) & ~4);
    *(u32 *)((u8 *)t3 + 0x114) = *(u32 *)((u8 *)t1 + 0x114);
    *(u32 *)((u8 *)p0 + 0x60) = 0;
    *(u32 *)((u8 *)p0 + 0x5c) = *(u32 *)((u8 *)p0 + 0x60);
    *(u32 *)((u8 *)p0 + 0x54) = *(u32 *)((u8 *)p0 + 0x5c);
    *(u32 *)((u8 *)p0 + 0x50) = *(u32 *)((u8 *)p0 + 0x54);
}
