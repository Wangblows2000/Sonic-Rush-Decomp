#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gNaviMsgTask;
void func_02053bec(BOOL on) { UnkWork *w; if (gNaviMsgTask == NULL) return; w = gNaviMsgTask->unk10; if (on) w->flags |= 0x1000000; else w->flags &= ~0x1000000; }
