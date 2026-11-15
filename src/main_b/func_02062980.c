#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u32 unk1C; s32 unk20; u32 unk24; } UnkElem;
typedef struct { u8 unk0[0xc]; u8 unkC; } UnkSub;
typedef struct { void (*fn)(void); u8 unk4[0xc]; u32 unk10; UnkSub sub; u8 unk21[0x47]; void *unk68; u8 unk6C[8]; u32 unk74; } UnkStruct_02062980;
UnkElem *func_02060470(void *p, u8 i);
void func_0206199c(UnkSub *p, u8 v);
void func_0206294c(void);
void func_02062980(UnkStruct_02062980 *s) { UnkElem *e = func_02060470(s->unk68, s->unk74); u32 *q = &e->unk1C; UnkSub *p = &s->sub; p->unkC = q[1]; func_0206199c(p, q[2]); s->unk10 = 4; s->fn = func_0206294c; }

#pragma thumb reset
