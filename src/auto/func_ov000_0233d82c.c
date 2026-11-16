/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 GX_ResetBankForBGExtPltt();
extern u8 gDisplayMain[];
s32 MIi_CpuCopy32();
s32 GX_SetBankForBGExtPltt();
s32 func_ov000_0233d25c();
extern u8 gObjDrawPalette[];
extern u8 gObjDrawTintPalette[];

void func_ov000_0233d82c(u8 *p0) {
    s32 t0 = GX_ResetBankForBGExtPltt();
    s32 t1 = *(u32 *)((u8 *)gDisplayMain + 0x84);
    MIi_CpuCopy32(*(u32 *)((u8 *)gDisplayMain + 0x84), ((u8 *)p0 + 0xac), 0x200);
    MIi_CpuCopy32(t1, ((u8 *)p0 + 0x2ac), 0x200);
    GX_SetBankForBGExtPltt(t0);
    func_ov000_0233d25c(p0, ((u8 *)p0 + 0x2ac));
    MIi_CpuCopy32(0x5000000, ((u8 *)p0 + 0x4ac), 0x200);
    MIi_CpuCopy32(0x5000000, ((u8 *)p0 + 0x6ac), 0x200);
    func_ov000_0233d25c(p0, ((u8 *)p0 + 0x6ac));
    *(u32 *)((u8 *)p0 + 0x8ac) = (u32)gObjDrawPalette;
    *(u32 *)((u8 *)p0 + 0x8b0) = (u32)gObjDrawTintPalette;
    func_ov000_0233d25c(p0, *(u32 *)((u8 *)p0 + 0x8b0));
}
