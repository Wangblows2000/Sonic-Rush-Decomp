#include <nitro/types.h>

typedef struct { u8 unk0[6]; u8 unk6[0x18]; } UnkElem;
typedef struct { u8 unk0[0x192c]; UnkElem arr[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
typedef struct { u8 unk0[0xa]; u8 unkA[6]; u16 unk10; u8 unk12[2]; u8 unk14[0x18]; } UnkOut_02042608;
void MI_CpuCopy8(const void *src, void *dst, u32 size);
BOOL WirelessMgr_JudgeAcceptChild(UnkOut_02042608 *o) { UnkElem *e = &gWirelessMgrTask->unk10->arr[o->unk10 - 1]; MI_CpuCopy8(o->unkA, e->unk0, 6); MI_CpuCopy8(o->unk14, e->unk6, 0x18); return TRUE; }
