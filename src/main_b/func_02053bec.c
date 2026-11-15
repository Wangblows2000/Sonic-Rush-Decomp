#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4660;
void func_02053bec(BOOL on) { UnkWork *w; if (data_022c4660 == NULL) return; w = data_022c4660->unk10; if (on) w->flags |= 0x1000000; else w->flags &= ~0x1000000; }
