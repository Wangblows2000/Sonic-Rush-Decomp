#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; u8 unkD8[0xc]; s32 unkE4; } UnkStruct_02052d44;
void func_02052e1c(UnkStruct_02052d44 *s);
void func_020537b4(s32 a);
void func_02052d44(UnkStruct_02052d44 *s) { if (!(s->flags & 0x100)) return; if (s->unkE4 <= 8) { func_02052e1c(s); return; } func_020537b4(10); }
