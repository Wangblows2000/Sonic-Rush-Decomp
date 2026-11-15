#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u32 unkC; } UnkSub_02045d64;
typedef struct { u8 unk0[0x330]; UnkSub_02045d64 sub; u32 unk340; u32 flags; } UnkStruct_02045d64;
void func_02045dec(void);
void func_02045d64(UnkStruct_02045d64 *s) { UnkSub_02045d64 *p = &s->sub; p->unk8 = 0; p->unkC = 0xc0000; s->flags |= 0xe0000; p->fn = func_02045dec; }

#pragma thumb reset
