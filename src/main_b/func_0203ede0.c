#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ede0;
void WH_SetError(s32 v);
void func_0203ede0(UnkStruct_0203ede0 *s) { if (s->unk2 == 0) return; WH_SetError(s->unk2); }
