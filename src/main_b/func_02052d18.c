#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; u8 unkD8[8]; u32 unkE0; u8 unkE4[0x20]; s32 unk104; } UnkStruct_02052d18;
void func_02052d18(UnkStruct_02052d18 *s) { s->unk104--; if (s->unk104 <= 0) { s->unkE0 = 0; s->flags |= 0x800000; } }
