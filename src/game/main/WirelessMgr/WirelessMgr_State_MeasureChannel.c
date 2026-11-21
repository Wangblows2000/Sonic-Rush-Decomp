#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u32 unkC; u8 unk10[0xa]; u16 unk1A; } UnkStruct_020422a4;
extern u8 gWirelessUserGameInfo;
u32 WH_GetSystemState(void);
void WH_SetUserGameInfo(void *a, u16 b);
BOOL WH_StartMeasureChannel(void);
void WirelessMgr_State_Error(void);
void WirelessMgr_State_WaitMeasureChannel(void);
void WirelessMgr_State_MeasureChannel(UnkStruct_020422a4 *s) { if (WH_GetSystemState() != 1) return; WH_SetUserGameInfo(&gWirelessUserGameInfo, 0x70); if (WH_StartMeasureChannel() == 0) { s->unkC = 1; s->fn = WirelessMgr_State_Error; } else { s->unk1A = 0; s->fn = WirelessMgr_State_WaitMeasureChannel; } }
