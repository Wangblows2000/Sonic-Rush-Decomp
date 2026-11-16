/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4364[];
s32 MIi_CpuCopy32();

void func_02042388(u8 *p0, u8 *p1) {
    s32 t0 = *(u8 *)((u8 *)p0 + 0x4b);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022c4364 + 0x10);
    if ((*(u8 *)((u8 *)p0 + 0x4b) & 2) != 0) {
        return;
    }
    if (*(u16 *)((u8 *)(t1 + 0x1900) + 0x28) != 0) {
        return;
    }
    MIi_CpuCopy32(p0, (t1 + 0x28), 0xc0, t0);
    *(u32 *)((u8 *)t1 + 0xe8) = 0;
    *(u16 *)((u8 *)t1 + 0xec) = *(u16 *)((u8 *)p1 + 0x12);
    *(u16 *)((u8 *)(t1 + 0x1900) + 0x28) = 1;
}
