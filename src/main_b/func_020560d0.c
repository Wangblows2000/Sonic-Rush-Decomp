#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { u32 unk0; UnkSub *unk4; } UnkStruct_020560d0;
typedef struct { void (*fn)(void); u32 unk4; u32 flags; } UnkObj_020560d0;
void func_02055eb4(void *a);
void func_02056084(void);
void func_020560d0(UnkStruct_020560d0 *s, UnkObj_020560d0 *o) { o->flags &= ~1; func_02055eb4(s); s->unk4->flags &= ~0x2000; o->fn = func_02056084; }

#pragma thumb reset
