#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205c6d0;
typedef struct { void (*fn)(void); u32 unk4; u32 unk8; } UnkObj_0205c6d0;
void func_0205c6ac(void);
void func_0205c6d0(UnkStruct_0205c6d0 *s, UnkObj_0205c6d0 *o) { s->flags &= ~2; s->flags2 |= 1; o->unk4 = 0; o->unk8 = 0; o->fn = func_0205c6ac; }
