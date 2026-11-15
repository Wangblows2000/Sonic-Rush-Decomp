#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205cf28;
void func_0205cf10(void *a, void *s);
void func_0205cf28(UnkStruct_0205cf28 *s, void (**p)(void *, void *)) { if (s->flags & 0x100000) *p = func_0205cf10; }
