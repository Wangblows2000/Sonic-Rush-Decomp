#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gNaviMsgTask;
BOOL NaviMsg_IsVisible(void) { if (gNaviMsgTask == NULL) return FALSE; return (gNaviMsgTask->unk10->flags & 1) != 0; }
