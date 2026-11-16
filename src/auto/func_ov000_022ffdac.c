/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjShift_SetFromRef();
s32 Math_LerpFx32();
s32 func_ov000_023008a0();
s32 func_ov000_02300808();

void func_ov000_022ffdac(u8 *p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x1e8) << 8);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x1ec) << 8);
    if (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x2) != 0x12) {
        s32 t2 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x2e), 0x1000, 0, 4, 0x280, 0x80);
        *(u16 *)((u8 *)p0 + 0x2e) = t2;
    } else {
        s32 t3 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x2e), 0x1000, 0, 2, 0x600, 0x100);
        *(u16 *)((u8 *)p0 + 0x2e) = t3;
    }
    *(u32 *)((u8 *)p0 + 0x1c) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x20) = *(u32 *)((u8 *)p0 + 0x14);
    s32 t4 = Math_LerpFx32(t0, *(u32 *)((u8 *)p0 + 0x1fc), *(u16 *)((u8 *)p0 + 0x2e));
    *(u32 *)((u8 *)p0 + 0x10) = t4;
    s32 t5 = Math_LerpFx32(t1, *(u32 *)((u8 *)p0 + 0x200), *(u16 *)((u8 *)p0 + 0x2e));
    *(u32 *)((u8 *)p0 + 0x14) = t5;
    if (*(u32 *)((u8 *)p0 + 0x10) != t0) {
        return;
    }
    if (*(u32 *)((u8 *)p0 + 0x14) != t1) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0x60;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t6 = *(u32 *)((u8 *)p0 + 0x1e0);
    if (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) == 0) {
        if (*(u8 *)((u8 *)t6 + 0x8) == 0) {
            func_ov000_023008a0(p0, t6);
            return;
        }
    }
    func_ov000_02300808(p0);
}
