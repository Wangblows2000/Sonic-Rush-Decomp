#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; u16 unk8CE; } UnkStruct_0205b24c;
void func_0205b228(void);
void func_0205b24c(UnkStruct_0205b24c *s) { s->unk8CC = 0; s->fn = func_0205b228; }
