#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u16 unk4; u16 unk6; u32 unk8; u32 unkC; } UnkStruct_02045ea8;
void ClearDemo_Unk_02045ea8(UnkStruct_02045ea8 *s) { s->unk4 = 0; s->unk6 = 0; s->unk8 = 0xf0000000; s->unkC = 0x100c0000; }

#pragma thumb reset
