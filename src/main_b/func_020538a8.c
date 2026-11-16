#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gNaviMsgTask;
BOOL func_020538a8(void) { if (gNaviMsgTask == NULL) return FALSE; return (gNaviMsgTask->unk10->flags & 1) != 0; }
