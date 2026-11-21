#include <nitro/types.h>

typedef struct { u8 unk0[0xc8]; } UnkElem;
typedef struct { u8 unk0[0x28]; UnkElem arr[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
UnkElem *WirelessMgr_GetScanResult(s32 i) { return &gWirelessMgrTask->unk10->arr[i]; }
