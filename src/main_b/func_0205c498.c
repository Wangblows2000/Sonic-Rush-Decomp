#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; u32 flags2; } UnkStruct_0205c498;
typedef struct { void (*fn)(void); u8 unk4[0x90]; s32 unk94; s32 unk98; u8 unk9C[2]; s16 unk9E; } UnkObj_0205c498;
void func_0205c470(void);
void func_0205c498(UnkStruct_0205c498 *s, UnkObj_0205c498 *o) { s->flags &= ~0x40; s->flags2 |= 8; o->unk94 = 0x1a0000; o->unk98 = -0x5c000; o->unk9E = 0; o->fn = func_0205c470; }
