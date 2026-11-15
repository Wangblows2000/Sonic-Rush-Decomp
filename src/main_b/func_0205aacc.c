#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; } UnkStruct_0205aacc;
void func_0205aa80(void);
void func_0205aacc(UnkStruct_0205aacc *s) { if (s->flags & 0x40000) s->fn = func_0205aa80; }
