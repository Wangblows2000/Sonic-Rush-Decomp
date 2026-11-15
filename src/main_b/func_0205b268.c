#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; u32 flags2; } UnkStruct_0205b268;
void func_0205b24c(void);
void func_0205b268(UnkStruct_0205b268 *s) { s->flags |= 0xa80; s->flags2 |= 0x70; s->fn = func_0205b24c; }
