#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u8 unkC[0xb]; u8 unk17; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
void WirelessMgr_RequestStart(void) { UnkWork *w = gWirelessMgrTask->unk10; if (w->unk8 == 2) w->unk17 = 1; }
