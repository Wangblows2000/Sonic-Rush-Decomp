#include <nitro/types.h>

typedef struct { s32 unk0; u8 unk4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
typedef struct { u32 unk0; u32 unk4; u8 unk8[0x40]; s32 unk48; u8 unk4C; u8 unk4D; } UnkStruct_022c4490;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4490 data_022c4490;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void func_0204b718(u32 v) { vu16 z = 0; MIi_CpuClear16(z, &data_022c4490, 0x60); data_022c4490.unk4 = v; data_022c4490.unk48 = data_022c4560.unk0; data_022c4490.unk4C = data_022c4560.unk1C; data_022c4490.unk4D = data_022c4560.unk1D; }
