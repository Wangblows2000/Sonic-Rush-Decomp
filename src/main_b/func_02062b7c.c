#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[0xd8]; s16 unkDC; } UnkStruct_02062b7c;
BOOL func_02061870(void *p);
void func_02061a78(void *p);
void TalkDemo_LoadCommonSprites(UnkStruct_02062b7c *s);
BOOL func_020610b8(UnkStruct_02062b7c *s, s16 *out);
void func_0206125c(UnkStruct_02062b7c *s);
void func_02062b5c(void);
void func_02062b7c(UnkStruct_02062b7c *s) { func_02061870(NULL); func_02061a78(NULL); TalkDemo_LoadCommonSprites(s); func_020610b8(s, &s->unkDC); func_0206125c(s); s->fn = func_02062b5c; }

#pragma thumb reset
