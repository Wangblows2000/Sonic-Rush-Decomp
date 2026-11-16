#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 unk8; u16 unkC; u8 unkE; u8 unkF; u8 unk10[0x54]; u16 unk64; } UnkStruct_022c4680;
extern UnkStruct_022c4680 gSaveData;
u32 func_02059b1c(void) { return gSaveData.unk8; }
