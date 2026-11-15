#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; u16 unk8CE; } UnkStruct_0205b35c;
void func_0205b2d0(void);
void func_0205b35c(UnkStruct_0205b35c *s) { s->unk8CE = 0; s->fn = func_0205b2d0; }
