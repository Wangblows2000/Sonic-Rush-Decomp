/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ExtraBossObj_State_Unk_ov001_022f011c();
extern u8 data_ov001_02316840[];
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();
extern u8 data_ov001_0231f074[];

void ExtraBossObj_State_Unk_ov001_022f0234(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18) + 0x84);
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_Unk_ov001_022f011c;
    Model_SetAnimation(t0, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0xb0), 0);
    Model_SetAnimation(t0, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0xb2), 0);
    *(u8 *)((u8 *)t0 + 0x10c) = (*(u8 *)((u8 *)t0 + 0x10c) & ~4);
    *(u8 *)((u8 *)t0 + 0x110) = (*(u8 *)((u8 *)t0 + 0x110) & ~4);
    *(u32 *)((u8 *)t0 + 0x114) = 0x1000;
    Model_SetAnimation(t1, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)data_ov001_02316840 + 0xb4), 0);
    Model_SetAnimation(t1, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)data_ov001_02316840 + 0xb6), 0);
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) & ~4);
    *(u8 *)((u8 *)t1 + 0x110) = (*(u8 *)((u8 *)t1 + 0x110) & ~4);
    *(u32 *)((u8 *)t1 + 0x114) = 0x1000;
}
