#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x10]; u32 flags; } UnkStruct_0205b148;
void func_0205b120(void);
void func_0205b148(UnkStruct_0205b148 *s) { if (s->flags & 0x10000) s->fn = func_0205b120; }
