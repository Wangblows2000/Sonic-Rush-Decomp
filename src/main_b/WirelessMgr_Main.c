#include <nitro/types.h>

typedef struct UnkWork { void (*fn)(struct UnkWork *); u8 unk4[8]; u32 unkC; u8 unk10[0xa]; u16 unk1A; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void WirelessMgr_State_Error(UnkWork *w);
void WirelessMgr_Main(void) { UnkWork *w = gTaskCurrent->unk10; if (w->unk1A == 480) { w->unk1A++; w->unkC = 3; w->fn = WirelessMgr_State_Error; } else if (w->unk1A < 480) { w->unk1A++; } w->fn(w); }
