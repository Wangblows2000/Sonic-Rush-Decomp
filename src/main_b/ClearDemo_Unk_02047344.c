#include <nitro/types.h>

#pragma thumb on

typedef struct UnkObj_02047344 { void (*fn)(void *, struct UnkObj_02047344 *); s32 unk4; s32 unk8; u16 unkC; } UnkObj_02047344;
typedef struct { u8 unk0[0x1c]; u32 unk1C; } UnkStruct_0208922c;
extern UnkStruct_0208922c gDisplayMain;
void ClearDemo_Unk_020472bc(void *a, UnkObj_02047344 *o);
void ClearDemo_Unk_02047344(void *a, UnkObj_02047344 *o) { o->unk4 = 0xdf000; o->unk8 = -0x23000; gDisplayMain.unk1C = 0; o->unkC = 0; o->fn = ClearDemo_Unk_020472bc; o->fn(a, o); }

#pragma thumb reset
