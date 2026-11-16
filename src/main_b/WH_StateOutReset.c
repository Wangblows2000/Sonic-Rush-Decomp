#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ebc8;
extern s32 data_022c196c;
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
void WH_CallbackForWFS(UnkStruct_0203ebc8 *s);
void WH_StateOutReset(UnkStruct_0203ebc8 *s) { if (s->unk2 != 0) { WH_ChangeSysState(9); WH_SetError(s->unk2); return; } if ((u32)(data_022c196c - 6) <= 1) WH_CallbackForWFS(s); WH_ChangeSysState(1); }
