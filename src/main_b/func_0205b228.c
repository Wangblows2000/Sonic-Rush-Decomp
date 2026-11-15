#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x454]; u16 unk8CC; } UnkStruct_0205b228;
void func_0205b1f8(void);
void func_0205b228(UnkStruct_0205b228 *s) { if (s->unk8CC++ == 30) s->fn = func_0205b1f8; }
