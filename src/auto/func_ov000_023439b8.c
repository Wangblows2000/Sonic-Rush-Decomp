/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjRect_SetBox3D();

void func_ov000_023439b8(u8 *p0, s32 p1, s32 p2) {
    if (*(u16 *)p0 != 7) {
        return;
    }
    s32 t0 = *(u16 *)((u8 *)p0 + 0x4);
    if ((u32)t0 > (u32)1) {
        return;
    }
    *(u32 *)((u8 *)(t0 * 0x2c + (s32)p2) + 0x2b8) = p2;
    ObjRect_SetBox3D((*(u16 *)((u8 *)p0 + 0x4) * 0x2c + (s32)((u8 *)p2 + 0x2a0)), *(s16 *)((u8 *)p0 + 0x8), *(s16 *)((u8 *)p0 + 0xa), *(s16 *)((u8 *)p0 + 0xc), *(s16 *)((u8 *)p0 + 0xe), -4, 4);
}
