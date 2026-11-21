#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_020402b0;
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
BOOL WH_StateInStartParent(void);
void WH_StateOutSetParentWEPKey(UnkStruct_020402b0 *s) { if (s->unk2 != 0) { WH_SetError(s->unk2); WH_ChangeSysState(9); return; } if (WH_StateInStartParent()) return; WH_ChangeSysState(9); }
