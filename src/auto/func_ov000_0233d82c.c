/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0200311c();
extern u8 data_0208922c[];
s32 MIi_CpuCopy32();
s32 func_02003788();
s32 func_ov000_0233d25c();
extern u8 data_022c5260[];
extern u8 data_022c5460[];

void func_ov000_0233d82c(u8 *p0) {
    s32 t0 = func_0200311c();
    s32 t1 = *(u32 *)((u8 *)data_0208922c + 0x84);
    MIi_CpuCopy32(*(u32 *)((u8 *)data_0208922c + 0x84), ((u8 *)p0 + 0xac), 0x200);
    MIi_CpuCopy32(t1, ((u8 *)p0 + 0x2ac), 0x200);
    func_02003788(t0);
    func_ov000_0233d25c(p0, ((u8 *)p0 + 0x2ac));
    MIi_CpuCopy32(0x5000000, ((u8 *)p0 + 0x4ac), 0x200);
    MIi_CpuCopy32(0x5000000, ((u8 *)p0 + 0x6ac), 0x200);
    func_ov000_0233d25c(p0, ((u8 *)p0 + 0x6ac));
    *(u32 *)((u8 *)p0 + 0x8ac) = (u32)data_022c5260;
    *(u32 *)((u8 *)p0 + 0x8b0) = (u32)data_022c5460;
    func_ov000_0233d25c(p0, *(u32 *)((u8 *)p0 + 0x8b0));
}
