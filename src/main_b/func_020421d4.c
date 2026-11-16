#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_020421d4;
extern u32 gSystemFlags;
u32 WH_GetSystemState(void);
void func_020421b4(void);
void func_020421d4(UnkStruct_020421d4 *s) { WH_GetSystemState(); if (WH_GetSystemState() != 5) return; gSystemFlags |= 0x300; s->unk1A = 0; s->fn = func_020421b4; }
