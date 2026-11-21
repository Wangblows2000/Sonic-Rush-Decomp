#include <nitro/types.h>

typedef struct { u8 unk0[0x1928]; u16 unk1928; } UnkStruct_02043300;
typedef struct { u8 unk0[0x10]; UnkStruct_02043300 *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u16 WirelessMgr_GetScanResultCount(void) { return gWirelessMgrTask->unk10->unk1928; }
