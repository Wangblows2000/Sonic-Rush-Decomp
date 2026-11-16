#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fb64;
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
void WH_StateOutEndParent(UnkStruct_0203fb64 *s) { if (s->unk2 != 0) { WH_SetError(s->unk2); return; } WH_ChangeSysState(1); }
