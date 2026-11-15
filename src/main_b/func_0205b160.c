#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; u32 flags2; } UnkStruct_0205b160;
void func_0205b148(void);
void func_0205b160(UnkStruct_0205b160 *s) { s->flags &= ~0x2000; s->flags2 &= ~0x80; s->flags |= 0x208000; s->flags2 |= 0x100; s->fn = func_0205b148; }
