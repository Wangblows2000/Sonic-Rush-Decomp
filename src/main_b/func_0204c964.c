#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
typedef struct { u8 unk0[0x58]; s16 unk58; s16 unk5A; u32 unk5C; } UnkStruct_022c4590;
typedef struct { u32 unk0; s32 unk4; s32 unk8; u8 unkC[0xbc]; u32 unkC8; } UnkPlayer;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;
extern UnkPlayer data_ov000_0236aad8;
void func_0204c964(void) { if (!(data_022c4560.unk18 & 0x40)) return; data_022c4590.unk58 = data_ov000_0236aad8.unk4 >> 8; data_022c4590.unk5A = data_ov000_0236aad8.unk8 >> 8; data_022c4590.unk5C = data_ov000_0236aad8.unkC8 & 2; }
