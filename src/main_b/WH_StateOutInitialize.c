#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e388;
void WH_SetError(s32 v);
void WH_ChangeSysState(s32 a);
void WH_IndicateHandler(void);
s32 func_0201154c(void (*cb)(void));
void WH_StateOutInitialize(UnkStruct_0203e388 *s) { s32 r; if (s->unk2 != 0) { WH_SetError(s->unk2); WH_ChangeSysState(10); return; } r = func_0201154c(WH_IndicateHandler); if (r != 0) { WH_SetError(r); WH_ChangeSysState(10); return; } WH_ChangeSysState(1); }
