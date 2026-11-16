#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xc]; u16 unk18; u16 unk1A; } UnkStruct_02041430;
void WirelessMgr_State_WaitStart(UnkStruct_02041430 *s) { if (s->unk18 != 0) s->unk18--; else s->unk8 = 6; s->unk1A = 0; }
