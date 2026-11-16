#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e44c;
void WH_ChangeSysState(s32 a);
void OS_Terminate(void);
void WH_IndicateHandler(UnkStruct_0203e44c *s) { if (s->unk2 != 8) return; WH_ChangeSysState(9); OS_Terminate(); }
