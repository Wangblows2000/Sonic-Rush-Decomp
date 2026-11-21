#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e9cc;
void WH_ChangeSysState(s32 a);
void WH_StateOutEnd(UnkStruct_0203e9cc *s) { if (s->unk2 != 0) { WH_ChangeSysState(10); return; } WH_ChangeSysState(0); }
