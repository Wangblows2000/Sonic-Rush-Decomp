#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4660;
BOOL func_020538a8(void) { if (data_022c4660 == NULL) return FALSE; return (data_022c4660->unk10->flags & 1) != 0; }
