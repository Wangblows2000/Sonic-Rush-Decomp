/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022ee198();
s32 func_ov000_022e9b70();
s32 func_ov000_023273f8();
s32 Sound_PlaySeqArc();
extern u8 data_ov000_02369ac0[];
void func_ov000_022e9038();

void func_ov000_022e908c(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    func_ov000_022ee198(p0, 0x32, 0);
    func_ov000_022e9b70(p0, 1, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0xbe) = 0x80;
    *(u16 *)((u8 *)p0 + 0x106) = 0x80;
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x124) + 0x114) = 0x8000;
    s32 t0 = func_ov000_023273f8(p0, *(u32 *)((u8 *)p0 + 0x3d0), -10, 0xe, 0);
    *(u32 *)((u8 *)p0 + 0x484) = t0;
    if (*(u32 *)((u8 *)p0 + 0x3d0) == 0) {
        Sound_PlaySeqArc(0, 0, 0);
    } else {
        Sound_PlaySeqArc(0, 0, 0x11);
    }
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x29;
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e9038;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x440))(p0, *(u32 *)((u8 *)p0 + 0x440), 0x29);
}
