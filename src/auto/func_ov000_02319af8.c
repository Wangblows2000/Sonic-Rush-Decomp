/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjSpd_UpSet();

void func_ov000_02319af8(u8 *p0) {
    if (*(s16 *)((u8 *)p0 + 0x428) > 0) {
        s32 t0 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x42a), -1, 0xc0);
        *(u16 *)((u8 *)p0 + 0x42a) = t0;
    } else {
        s32 t1 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x42a), 1, 0xc0);
        *(u16 *)((u8 *)p0 + 0x42a) = t1;
    }
    *(u16 *)((u8 *)p0 + 0x428) = (*(s16 *)((u8 *)p0 + 0x428) + *(s16 *)((u8 *)p0 + 0x42a));
    if ((u32)*(u32 *)((u8 *)p0 + 0x8) > (u32)0x1a00) {
        s32 t2 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x424), -256, 0x30);
        *(u16 *)((u8 *)p0 + 0x424) = t2;
    } else {
        s32 t3 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x424), 0x100, 0x30);
        *(u16 *)((u8 *)p0 + 0x424) = t3;
    }
    *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + *(s16 *)((u8 *)p0 + 0x424));
}
