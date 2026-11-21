#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u32 WirelessMgr_GetErrorCode(void) { if (gWirelessMgrTask == NULL) return 0; return gWirelessMgrTask->unk10->unkC; }
