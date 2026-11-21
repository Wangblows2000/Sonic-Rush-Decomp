#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; s32 unkC; u32 unk10; u8 unk14[0x22]; u16 unk36; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
void WirelessLobbyChild_SelectParent(u16 a) { UnkTask *t = gWirelessLobbyTask; UnkWork *w = t->unk10; s32 v; if (t == NULL) v = 7; else v = w->unkC; if (v == 3) { w->unk36 = a; w->unk10 = 1; } }

#pragma thumb reset
