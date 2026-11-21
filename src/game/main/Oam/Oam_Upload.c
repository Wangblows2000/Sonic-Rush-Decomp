/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayTable[];
extern u8 gOamAddrTable[];
extern u8 data_02073204[];
s32 MI_DmaCopy32();
s32 MIi_CpuCopyFast();

void Oam_Upload(s32 p0) {
    s32 t0 = *(u32 *)((u8 *)gDisplayTable + ((s32)p0 << 2));
    MI_DmaCopy32(3, (*(u32 *)((u8 *)gDisplayTable + ((s32)p0 << 2)) + 0x8a8), *(u32 *)((u8 *)gOamAddrTable + ((s32)p0 << 2)), *(u32 *)((u8 *)data_02073204 + ((s32)p0 << 2)));
    MIi_CpuCopyFast((t0 + 0xca8), (t0 + 0x8a8), 0x400);
}
