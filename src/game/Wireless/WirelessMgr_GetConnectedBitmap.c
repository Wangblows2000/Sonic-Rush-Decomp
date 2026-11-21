#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u16 WirelessMgr_GetConnectedBitmap(void) { return gWirelessMgrTask->unk10[10]; }
