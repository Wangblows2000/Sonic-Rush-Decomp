#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205c5b8;
typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; s32 unk50; s32 unk54; } UnkObj_0205c5b8;
void func_0205c590(void);
void func_0205c5b8(UnkStruct_0205c5b8 *s, UnkObj_0205c5b8 *o) { s->flags &= ~8; s->flags |= 0x10; s->flags2 |= 4; o->unk4C = 0; o->unk50 = 0xb4000; o->unk54 = 0x2000; o->fn = func_0205c590; }
