/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjRect_CenterX();
s32 ObjRect_CenterY();
s32 func_ov000_02354c00();

void func_ov000_023146b0(u8 *p0, u8 *p1) {
    if (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x18) + 0x204) != 0) {
        return;
    }
    if (*(u32 *)((u8 *)p1 + 0x18) == 0) {
        return;
    }
    if (*(u8 *)*(u32 *)((u8 *)p1 + 0x18) != 1) {
        return;
    }
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)p1 + 0x18) + 0x58) & 0x10) != 0) {
        return;
    }
    s32 t0 = *(s16 *)((u8 *)(*(u32 *)((u8 *)p1 + 0x18) + 0x300) + 0x7c);
    s32 t1 = *(s16 *)((u8 *)*(u32 *)((u8 *)p1 + 0x18) + 0x34);
    s32 t2 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t3 = *(u32 *)((u8 *)p1 + 0x18);
    s32 v4 = t1;
    if (t1 < 0) {
        v4 = -v4;
    }
    if (v4 < t0) {
        return;
    }
    *(u32 *)((u8 *)t2 + 0x204) = t3;
    s32 t5 = (*(u32 *)((u8 *)t2 + 0x54) | 2);
    *(u32 *)((u8 *)t2 + 0x54) = t5;
    s32 t6 = ObjRect_CenterX((t2 + 0x134), t5);
    s32 t7 = ObjRect_CenterY((t2 + 0x134));
    func_ov000_02354c00(t3, t2, t6, t7);
}
