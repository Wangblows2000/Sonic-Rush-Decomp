#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_02041238;
u32 WH_GetSystemState(void);
void WirelessMgr_State_DownloadPlayStartMB(void);
void WirelessMgr_State_DownloadPlayWaitStop(UnkStruct_02041238 *s) { if (WH_GetSystemState() == 0) { s->unk1A = 0; s->fn = WirelessMgr_State_DownloadPlayStartMB; } }
