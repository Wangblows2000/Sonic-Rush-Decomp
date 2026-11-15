#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ede0;
void func_020405bc(s32 v);
void func_0203ede0(UnkStruct_0203ede0 *s) { if (s->unk2 == 0) return; func_020405bc(s->unk2); }
