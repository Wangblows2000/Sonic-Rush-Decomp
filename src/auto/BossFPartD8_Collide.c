/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 BossFPartD8_Unk_ov000_022f53d0();
s32 ObjCollisionMap_Register();

void BossFPartD8_Collide(void) {
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x54) & 0xc) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    BossFPartD8_Unk_ov000_022f53d0(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x20c), (*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x340));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x238), (t0 + 0x3d0));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x264), (t0 + 0x3d0));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x290), (t0 + 0x3d0));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x2bc), (t0 + 0x400));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x2e8), (t0 + 0x400));
    BossFPartD8_Unk_ov000_022f53d0(t0, (t0 + 0x314), (t0 + 0x400));
    if (*(u32 *)((u8 *)t0 + 0x1b8) == 0) {
        return;
    }
    ObjCollisionMap_Register((t0 + 0x1b8));
}
