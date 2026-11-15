#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u8 unk4[9]; s8 unkD; } UnkStruct_02056d8c;
u32 func_02057014(void);
void *func_02056f90(void);
void func_02056b8c(UnkStruct_02056d8c *s, void *p);
void func_02056c98(void);
void func_02056d54(void);
void func_02056d8c(UnkStruct_02056d8c *s) { s->unkD = func_02057014(); if (s->unkD == 0) { s->fn = func_02056c98; return; } func_02056b8c(s, func_02056f90()); s->fn = func_02056d54; }

#pragma thumb reset
