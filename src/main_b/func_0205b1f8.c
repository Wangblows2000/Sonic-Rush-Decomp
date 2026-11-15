#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; u32 flags2; } UnkStruct_0205b1f8;
void func_0205b1e0(void);
void func_0205b1f8(UnkStruct_0205b1f8 *s) { s->flags |= 0x102000; s->flags2 |= 0x80; s->fn = func_0205b1e0; }
