#include <nitro/types.h>

#pragma thumb on

typedef struct { void (*fn)(void); u32 flags; u8 unk8[8]; u32 unk10; u8 unk14[0x6a]; u16 unk7E; } UnkStruct_02062794;
void func_02062738(void);
void func_02062794(UnkStruct_02062794 *s) { if (s->flags & 1) s->flags |= 4; s->flags &= ~0x10; s->unk10 = 10; s->unk7E = 0; s->fn = func_02062738; }

#pragma thumb reset
