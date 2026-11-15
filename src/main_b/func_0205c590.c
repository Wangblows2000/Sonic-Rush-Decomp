#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205c590;
typedef struct { void (*fn)(void); u8 unk4[0x48]; s32 unk4C; u32 unk50; s32 unk54; } UnkObj_0205c590;
void func_0205c538(void);
void func_0205c590(UnkStruct_0205c590 *s, UnkObj_0205c590 *o) { o->unk4C -= o->unk54; if (s->flags & 0x10000000) o->fn = func_0205c538; }
