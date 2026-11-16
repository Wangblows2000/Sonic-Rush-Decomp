/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_ov000_02366b3c[];
s32 func_0206d3fc();
s32 func_0206a054();
s32 func_02069818();

void func_ov000_0230d804(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x88) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x14);
    *(u32 *)((u8 *)p0 + 0x10) = (*(u32 *)((u8 *)p0 + 0x10) + (*(s16 *)((u8 *)p0 + 0x2e) << 8));
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) + (*(s16 *)((u8 *)p0 + 0x30) << 8));
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    if (*(u16 *)((u8 *)p0 + 0x9c) == (*(u8 *)((u8 *)data_ov000_02366b3c + (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7)) + 0x2)) {
        return;
    }
    s32 t0 = (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7);
    s32 t1 = *(u16 *)((u8 *)p0 + 0x9c);
    if (t1 == 0xa) {
        func_0206d3fc((*(u16 *)((u8 *)p0 + 0xd8) & 0xff), t0, t1);
        func_0206a054(p0, 0, 2);
    }
    func_02069818(p0, ((u32)((*(u8 *)((u8 *)data_ov000_02366b3c + (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7)) + 0x2) << 0x10) >> 0x10));
    if (*(u8 *)((u8 *)data_ov000_02366b3c + (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7)) != 8) {
        return;
    }
    func_0206d3fc((*(u16 *)((u8 *)p0 + 0xd8) & 0xff), (((s32)*(u32 *)((u8 *)p0 + 0xc) >> 5) & 7));
    func_0206a054(p0, 8, 0x17);
}
