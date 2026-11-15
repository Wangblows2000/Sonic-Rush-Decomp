#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; u8 unkD8[8]; u32 unkE0; u8 unkE4[0x20]; s32 unk104; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4660;
void func_02053874(void) { UnkWork *w; if (data_022c4660 == NULL) return; w = data_022c4660->unk10; w->unk104 = 0; w->unkE0 = 0; w->flags |= 0x800000; }
