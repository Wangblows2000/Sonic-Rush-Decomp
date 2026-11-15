#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205b9d4;
typedef struct { void (*fn)(void); u8 unk4[0xa0]; s32 unkA4; } UnkObj_0205b9d4;
void func_0205b994(void);
void func_0205b9d4(UnkStruct_0205b9d4 *s, UnkObj_0205b9d4 *o) { o->unkA4 += -0x333; if (s->flags & 0x10000000) o->fn = func_0205b994; }
