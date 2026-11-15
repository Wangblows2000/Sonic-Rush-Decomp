#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x28]; u32 unk28; } UnkCfg_02062860;
typedef struct { void (*fn)(void); u8 unk4[8]; u32 unkC; u8 unk10[4]; u8 unk14[0x58]; UnkCfg_02062860 *unk6C; } UnkStruct_02062860;
void func_0206195c(void *p, u8 v);
void func_02062834(void);
void func_02062860(UnkStruct_02062860 *s) { func_0206195c(s->unk14, s->unk6C->unk28); s->unkC = 0; s->fn = func_02062834; }

#pragma thumb reset
