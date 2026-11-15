#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c538;
typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; u32 unk50; s32 unk54; } UnkObj_0205c538;
void func_0205c4ec(void);
void func_0205c538(UnkStruct_0205c538 *s, UnkObj_0205c538 *o) { o->unk54 += 0x100; if (o->unk54 > 0x8000) o->unk54 = 0x8000; o->unk4C -= o->unk54; if (-o->unk54 <= o->unk4C) { s->flags &= ~0x10; o->fn = func_0205c4ec; } }
