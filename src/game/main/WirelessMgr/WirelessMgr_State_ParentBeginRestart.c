#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_020420fc;
void WirelessMgr_State_ParentRestartFinalize(void);
void WirelessMgr_State_ParentBeginRestart(UnkStruct_020420fc *s) { s->unk8 = 2; s->unk1A = 0; s->fn = WirelessMgr_State_ParentRestartFinalize; }
