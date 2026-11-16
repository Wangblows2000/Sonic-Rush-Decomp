/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_02304f50();

void func_ov000_02304fa0(u8 *p0) {
    func_02069818(p0, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02304f50;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x28) = 0;
    *(u16 *)((u8 *)p0 + 0x2a) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~2);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
}
