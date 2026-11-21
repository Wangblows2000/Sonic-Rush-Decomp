/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4ddc[];
extern u8 data_022c4de0[];
extern u8 data_022c4de4[];

void ObjCollisionMap_Register(u8 *p0) {
    if ((u32)*(u8 *)data_022c4ddc >= (u32)0x20) {
        return;
    }
    if ((*(u32 *)((u8 *)*(u32 *)p0 + 0x54) & 0xc) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4);
    s32 t1 = *(u32 *)p0;
    if (t0 != 0) {
        s32 t2 = *(u32 *)((u8 *)t0 + 0x80);
        if (t2 != t1) {
            *(u32 *)((u8 *)p0 + 0x4) = 0;
        }
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x8);
    if (t3 != 0) {
        s32 t4 = *(u32 *)p0;
        if (*(u32 *)((u8 *)t3 + 0x84) != t4) {
            *(u32 *)((u8 *)p0 + 0x8) = 0;
        }
    }
    s32 t5 = (*(u8 *)data_022c4de0 + 0x1);
    *(u32 *)((u8 *)data_022c4de4 + (*(u8 *)data_022c4de0 << 2)) = (u32)p0;
    *(u8 *)data_022c4de0 = t5;
}
