#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
typedef struct { u8 unk0[0x58]; s16 unk58; s16 unk5A; u32 unk5C; } UnkStruct_022c4590;
typedef struct { u32 unk0; s32 unk4; s32 unk8; u8 unkC[0xbc]; u32 unkC8; } UnkPlayer;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4590 data_022c4590;
extern UnkPlayer gCamera;
void func_0204c964(void) { if (!(gGameState.unk18 & 0x40)) return; data_022c4590.unk58 = gCamera.unk4 >> 8; data_022c4590.unk5A = gCamera.unk8 >> 8; data_022c4590.unk5C = gCamera.unkC8 & 2; }
