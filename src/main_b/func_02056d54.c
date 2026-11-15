#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[8]; s8 unkC; u8 unkD[0x47]; u16 unk54; u16 unk56; } UnkStruct_02056d54;
void *func_02056fa4(s32 i);
void func_02056bd4(UnkStruct_02056d54 *s, void *p);
void func_02056b74(void);
void func_02056d2c(void);
void func_02056d54(UnkStruct_02056d54 *s) { func_02056bd4(s, func_02056fa4(s->unkC)); func_02056b74(); s->unk54 = 0; s->unk56 = 6; s->fn = func_02056d2c; }

#pragma thumb reset
