#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fd30;
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
void func_0203fd30(UnkStruct_0203fd30 *s) { if (s->unk2 == 0) return; WH_SetError(s->unk2); WH_ChangeSysState(9); }
