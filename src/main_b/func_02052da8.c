#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkStruct_02052da8;
void func_02052e1c(UnkStruct_02052da8 *s);
void func_02052da8(UnkStruct_02052da8 *s) { if (!(s->flags & 0x100)) return; func_02052e1c(s); }
