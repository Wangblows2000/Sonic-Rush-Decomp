#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ece8;
void WH_SetError(s32 v);
void WH_Finalize(void);
BOOL WH_StateInEndChild(void);
void WH_ChangeSysState(s32 a);
void WH_StateOutEndChildMP(UnkStruct_0203ece8 *s) { if (s->unk2 != 0) { WH_SetError(s->unk2); WH_Finalize(); return; } if (WH_StateInEndChild()) return; WH_ChangeSysState(9); }
