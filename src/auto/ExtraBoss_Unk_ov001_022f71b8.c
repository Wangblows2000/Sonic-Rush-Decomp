/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f080[];
extern u8 data_ov001_023169b4[];
s32 MIi_CpuCopy16();

void ExtraBoss_Unk_ov001_022f71b8(s32 p0) {
    MIi_CpuCopy16(((s32)data_ov001_023169b4 + (p0 * 0x44)), (*(u32 *)data_ov001_0231f080 + (p0 * 0x44)), 0x44, data_ov001_023169b4);
}
