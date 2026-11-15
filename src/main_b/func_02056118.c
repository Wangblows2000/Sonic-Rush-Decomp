#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 flags; } UnkSub;
typedef struct { u32 unk0; UnkSub *unk4; } UnkStruct_02056118;
void func_02055eb4(void *a);
void func_02056118(UnkStruct_02056118 *s, u32 *b) { func_02055eb4(s); *b = 0; s->unk4->flags &= ~0x7000; }

#pragma thumb reset
